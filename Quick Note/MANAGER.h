#pragma once
#include "stdafx.h"
#define NOTE_DATA_FOLDER	L"NoteData/"
#define TAG_PATH			L"TagData/Tags.txt"
class MANAGER
{
public:
	vector<NOTE> NoteList;
	vector<TAG> TagList;
private:
	wstring standardizedTag(wstring tag)
	{
		while (tag.size() > 0)
		{
			if (tag[0] == ' ')
			{
				tag.erase(0, 1);
			}
			else
			{
				break;
			}
		}

		while (tag.size() > 1)
		{
			if (tag[tag.size() - 1] == ' ')
			{
				tag.pop_back();
			}
			else
			{
				break;
			}
		}

		
		return tag;
	}

public:
	
	vector<wstring> strToTagList(wstring rawTag)
	{
		//add defaul tag
		wstringstream ss;
		ss << ".All notes, " << rawTag;
		rawTag = ss.str();

		vector<wstring> tagList;
		wstringstream ssRawTag(rawTag);
		wstring singleTag;
	

		bool duplicated = false;
		while(getline(ssRawTag, singleTag, L','))
		{
			singleTag = standardizedTag(singleTag);
			
			if (singleTag != L"")
			{
				//check duplicated
				duplicated = false;
				for (int i = 0; i < tagList.size(); i++)
				{
					if (singleTag == tagList[i])
					{
						duplicated = true;
						break;
					}
				}
				if (!duplicated)
				{
					tagList.push_back(singleTag);
				}
			}
			
		}

		return tagList;

	}
	int saveNoteToList(vector<wstring> tags, wstring content, int currNoteId = -1)
	{
		//save tags
		bool existed = false;
		for (int tagI = 0; tagI < tags.size(); tagI++)
		{
			//check if tag existed
			existed = false;
			for (int tagListI = 0; tagListI < TagList.size(); tagListI++)
			{
				//if existed, push new note id to tag
				if (tags[tagI] == TagList[tagListI].TagName)
				{
					if (currNoteId == -1)//create new note
					{
						TagList[tagListI].Id.push_back(NoteList.size());
					}
					existed = true;
					break;
				}
			}

			// if tag is not exist in tags list, push new tag
			if (existed == false)
			{
				if (currNoteId != -1)
				{
					TAG newTag(tags[tagI], currNoteId);
					TagList.push_back(newTag);

				}
				else
				{
					TAG newTag(tags[tagI], NoteList.size());
					TagList.push_back(newTag);

				}
			}
		}

		//Save note
		if (currNoteId == -1)//create new note
		{
			NOTE newNote(NoteList.size(), tags, content);
			NoteList.push_back(newNote);

		}
		else//override
		{
			NOTE newNote(currNoteId, tags, content);
			NoteList[currNoteId] = newNote;
		}
		return 0;
	}

	//becareful cos this fuction "rewrite" all data a time, not "update"
	int saveToFile()
	{
		//save Tags data
		wofstream fileTags;
		wofstream fileNotes;
		fileTags.open(TAG_PATH);

		if (!fileTags.is_open())
		{
			return 1;
		}

		fileTags.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
		for (int i = 0; i < TagList.size(); i++)
		{
			fileTags << TagList[i].TagName << endl;
			for (int j = 0; j < TagList[i].Id.size(); j++)
			{
				fileTags << TagList[i].Id[j] << " ";
			}
			fileTags << endl;
		}
		fileTags.close();

		//write notes
		for (int i = 0; i < NoteList.size(); i++)
		{
			wstringstream ss;
			ss << NOTE_DATA_FOLDER << NoteList[i].Id << ".txt";

			fileNotes.open(ss.str());
			if (!fileNotes.is_open())
			{
				return 2;
			}

			fileNotes.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

			for (int j = 0; j < NoteList[i].Tag.size(); j++)
			{
				fileNotes << NoteList[i].Tag[j] <<",";
			}
			fileNotes << endl;
			fileNotes << NoteList[i].Content;

			fileNotes.close();

		}

		return 0;
	}

	int loadFromFile()
	{
		wfstream fileTag;
		fileTag.open(TAG_PATH);
		if (!fileTag.is_open())
		{
			return 1;
		}
		fileTag.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

		wstring tagName, id, idListStr;
		wstringstream  idList;
		while (getline(fileTag, tagName))
		{
			getline(fileTag, idListStr);
			//copy to stringstream to extract id
			std::wstringstream().swap(idList);
			idList << idListStr;

			TAG currTag(tagName);
			while (getline(idList, id, L' '))
			{
				currTag.Id.push_back(wcstod(id.c_str(), NULL));
			}

			TagList.push_back(currTag);
		}
		fileTag.close();
		

		//read notes file
		wfstream fileNote;

		HANDLE hFind;
		WIN32_FIND_DATA data;

		wstringstream pathSS, tempSS;
		wstring tagsString, tempStr;
		pathSS << NOTE_DATA_FOLDER << "*.txt";
		hFind = FindFirstFile(pathSS.str().c_str(), &data);
		if (hFind != INVALID_HANDLE_VALUE) {
			do
			{
				wstringstream().swap(pathSS);
				pathSS << NOTE_DATA_FOLDER << data.cFileName;
				fileNote.open(pathSS.str());
				if (!fileNote.is_open())
				{
					return 2;
				}


				fileNote.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
				getline(fileNote, tagsString);

				wstringstream().swap(tempSS);//reset stringstream
				while (getline(fileNote, tempStr))
				{
					tempSS << tempStr;
				}
				
				id = data.cFileName;
				id[id.size() - 4] = '\0'; //delete ".txt"
				NOTE currNote(wcstol(id.c_str(), NULL, 10), strToTagList(tagsString), tempSS.str());
				NoteList.push_back(currNote);

				fileNote.close();
			} while (FindNextFile(hFind, &data));
			FindClose(hFind);
		}

		
		return 0;
	}
};

