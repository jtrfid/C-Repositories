
var rno = "";
$(function () {
    $("#favit1").click(function () {
        var rno = $("#hdrno").val();
        $.post("/service/course.aspx", { act: 'favorite', rno: rno }, function (data, textStatus) {
            if (data == "1") {
                $("#favit1").hide();
                $("#favit2").show();
            }
            else {
                if (data != "0")
                    alert(data);
            }
        });
    });
    $("#likeit").click(function () {
        var rno = $("#hdrno").val();
        if ($("#haslike").text() != "")
        {
            $("#haslike").show();
            closeit();
            return;
        }
        $.post("/service/course.aspx", {act:"likeit",rno:rno}, function (data, textStatus) {
            if (data != "") {
                $("#likecount").text(data);
                $("#haslike").text("已经 Like 过了！");
            }
        });
    });
    rno = $("#hdrno").val();
    $.post("/service/course.aspx", { act: "getlike", rno: rno }, function (data, textStatus) {
        if (data.substring(0, 1) == "-")
        {
            $("#haslike").text("您已经 Like 过了！");
        }
        $("#likecount").text(data.substring(1));
    });
    $(".rbclose").click(function () {
        $(this).parent().hide('fast');
    });
    $(".rbtitle").mouseover(function () {
        $(this).next().show('fast');
    });
});
function showall() {
    $("#des").hide();
    $("#des2").show();
}
var closetime = 2;
function closeit() {
    setTimeout(function () {
        closetime--;
        if (closetime <= 0) {
            $("#haslike").hide();
        }
        else {
            closeit();
        }
    }, 1000);
}

//视频公开课
function calldata() {
	var name = $("#coursename").text();
	var namecn = $("#coursenamecn").text();
	var category = $("#hdcategory").val();
	var instructor = $("#hdteacher").val();
	$.getJSON("http://www.metel.cn:81/service/srv.aspx?code=" + category + "&name=" + name + "&namecn=" + namecn + "&instructor=" + instructor + "&jsoncallback=?");
}
//视频公开课回调函数
function showdata(data)
{
	var h = "";
	var vcode = $("#hdvcode").val();
	if (data.length > 0)
	{
		for (var i = 0; i < data.length && i < 5; i++)
		{
			h += "<div class='relcourse'><a href='http://www.metel.cn:81/views/course.aspx?rno=" + data[i].rno + "&code=" + escape(vcode) + "' target='_blank'>" + data[i].coursenamecn + "</a></div>";
			h += "<div class='relinfo'><span>"+data[i].school+"</span></div>";
		}
	}
	if (data.length > 0) {
		var html = "<div class='smalltitle'>相关的视频公开课</div>";
		html += "<div class='smallcontent'><div>";
		html += h;
		html += "</div></div>";
		$("#divrelopencourse").html(html);
	}

}