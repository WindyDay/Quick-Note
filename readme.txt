MSSV: 1412465

Họ tên: Nguyễn Đình Sơn

Email: 001.IceTea@gmail.com

SĐT: 0166 831 3970

Danh sách chức năng (Quick note):
	+ Nhập, lưu note kèm các tag (tag nhập vào phân cách bởi dấu phẩy ',')
	+ Duyệt các note với tag tương ứng
	+ Bắt phím "Window + space" để hiển thị khung nhập note
	+ Biểu đồ thống kê top 5 tag được sử dụng nhiều nhất
	+ Icon taskbar, chuột phải để thấy chức năng View notes, View statistics, Exit. Click đúp để view note
	+ Nhập tag nhanh với "Tag suggest", danh sách tag nhập vào được sắp xếp theo mức độ sử dụng
	
Dòng sự kiện chính:
	Ex1:
	+ Chạy ứng dụng
	+ Nhập "Hôm nay tôi nộp bài" vào khung Current Note
	+ Click nút Save
	+ Message box báo "Saved" xuất hiện, click "OK"
	+ Một tag mặc định tên ".All notes" sẽ xuất hiện bên trong "Tags List" 
	với chỉ số "(1)" phía sau là số note có tag này (Mặc định đều mang tag ".All notes")
	+ Click vào ".All notes(1)"
	+ Notes list sẽ xuất hiện dòng "Hôm nay tôi nộp bài..."
	+ Click vào "Hôm nay tôi nộp bài..." trong "Note list"
	+ Current note sẽ hiển thị "Hôm nay tôi nộp bài"
	+ Thêm "tag 1" vào khung tag, nhấn Save
	+ "Tag list" có thêm dòng "tag 1(1)"
	+ Click ".All notes(1)" => "Hôm nay tôi nộp bài..."
	+ khung tag được cập nhật thành "tag 1, "
	------
	+ Click "New note"
	+ "Current note" và khung "Tag" được xóa sạch
	+ Nhập "Ngày mai vẫn phải đi học" vào "Current Note"
	+ Nhập "tag 2" vào khung tag
	+ Nhấn nút xổ xuống của khung tag, chọn "tag 1"
	+ Khung tag chuyển thành "tag 2, tag 1, "
	+ Nhấn Save = > OK
	+ Tags list bây giờ bao gồm ".All notes(2)", "tag 1(2)", "tag 2(1)"
	-----------
	+ Nhấn Statistics
	+ Dialog Statistics xuất hiện với biểu đồ ngang cho thấy "tag 1 có 2 note (67%)" 
	và "tag 2 có 1 note (33%)
	+ Nhấn OK để tắt dialog
	-----------
	+ Chuột phải vào icon dưới notification bar => exit
	+ Chạy lại ứng dụng
	+ Ta thấy các thông tin về tag, note vẫn được lưu lại
	
	Ex2:
	+ Chạy ứng dụng
	+ Nhấn nút close phía góc trên phải cửa sổ
	+ Cửa sổ chính của ứng dụng biến mất
	+ Nhấn "Window + Space"
	+ Cửa sổ ứng dụng được mở lên
	
	Ex3:
	+ Chạy ứng dụng
	+ Nhấn nút close phía góc trên phải cửa sổ
	+ Cửa sổ chính của ứng dụng biến mất
	+ Chuột phải (hoặc click đúp) vào notification icon của ứng dụng => View note
	+ Cửa sổ ứng dụng được mở lên
		
	Ex3:
	+ Chạy ứng dụng
	+ Chuột phải vào notification icon của ứng dụng => View statistics
	+ Dialog statistics được mở lên
	
	Ex4:
	+ Chạy ứng dụng
	+ Chuột phải vào notification icon của ứng dụng => Exit
	+ Ứng dụng bị tắt hoàn toàn
	

Dòng sự kiện phụ:
	Ex1:
	+ Chạy ứng dụng
	+ Gõ "qwerty" vào Current note, "tag x,    tag y,   tag y, , , " vào khung tag
	+ Nhấn Save => OK
	+ Mở "tag x" => "qwerty..."
	+ ta thấy khung tag hiển thị "tag x, tag y," (các tag trùng  và dấu cách thừa bị loại bỏ)
	
	
	
Screen recording:
https://www.youtube.com/watch?v=b4R_ieBxTSk
		
IDE: Visual Studio 2015