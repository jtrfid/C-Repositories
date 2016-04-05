jQuery.TCookie = function (name, value, options) {
    if (typeof value != 'undefined') { // name and value given, set cookie
        options = options || {};
        if (value === null) {
            value = '';
            options.expires = -1;
        }
        var expires = '';
        if (options.expires && (typeof options.expires == 'number' || options.expires.toUTCString)) {
            var date;
            if (typeof options.expires == 'number') {
                date = new Date();
                date.setTime(date.getTime() + (options.expires * 24 * 60 * 60 * 1000));
            } else {
                date = options.expires;
            }
            expires = '; expires=' + date.toUTCString(); // use expires attribute, max-age is not supported by IE
        }
        // CAUTION: Needed to parenthesize options.path and options.domain
        // in the following expressions, otherwise they evaluate to undefined
        // in the packed version for some reason...
        var path = options.path ? '; path=' + (options.path) : '';
        var domain = options.domain ? '; domain=' + (options.domain) : '';
        var secure = options.secure ? '; secure' : '';
        document.cookie = [name, '=', encodeURIComponent(value), expires, path, domain, secure].join('');
    } else { // only name given, get cookie
        var cookieValue = null;
        if (document.cookie && document.cookie != '') {
            var cookies = document.cookie.split(';');
            for (var i = 0; i < cookies.length; i++) {
                var cookie = jQuery.trim(cookies[i]);
                // Does this cookie string begin with the name we want?
                if (cookie.substring(0, name.length + 1) == (name + '=')) {
                    cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                    break;
                }
            }
        }
        return cookieValue;
    }
};
var gdoptions = { path: '/', expires: 3 };
function AboutCookie(name, value) {
    $.TCookie(name, value, gdoptions);
}
function AboutCookie2(name, value) {
    $.TCookie(name, value, { path: '/', expires: 0 });
}

function SetBkColor(o) {
    var $d = $("#" + o);
    $d.attr("class", "b");
    if (o != "nlg") {
        $("#nlg").hide();
    }
    else {
        $("#nlg").show();
    }
    if (o != "nyk") {
        $("#nyk").hide();
    }
    if (o != "njg") {
        $("#njg").hide();
    }
    if (o != "nkc") {
        $("#nkc").hide();
    }
    else {
        $("#nkc").show();
    }
    if (o != "nzypx") {
        $("#nzypx").hide();
    }


    //var u = $.TCookie("GDUID");
    //if (u) {
    //    if (u == "admin") {
    //        $("#ngl").show();
    //    }
    //}    
}
function showlayer(obj) {
    var $o = $($(obj).parent().children()[0]);
    if ($o.attr("src").indexOf("add") > -1) {
        $o.parent().next().children().show('fast');

        $o.attr("src", "images/del.jpg");
    }
    else {
        $o.parent().next().children().hide('fast');

        $o.attr("src", "images/add.jpg");
    }
}

function DrawImage(ImgD, FitWidth, FitHeight) {
    var image = new Image();
    image.src = ImgD.src;
    if (image.width > 0 && image.height > 0) {
        if (image.width / image.height >= FitWidth / FitHeight) {
            if (image.width > FitWidth) {
                ImgD.width = FitWidth;
                ImgD.height = (image.height * FitWidth) / image.width;
            } else {
                ImgD.width = image.width;
                ImgD.height = image.height;
            }
        } else {
            if (image.height > FitHeight) {
                ImgD.height = FitHeight;
                ImgD.width = (image.width * FitHeight) / image.height;
            } else {
                ImgD.width = image.width;
                ImgD.height = image.height;
            }
        }
    }
}
$(function () {
    $("#nav A").hover(function () {
        $(this).css({ "background": "#f50", "color": "#ff0" });
    }, function () {
        $(this).css({ "background": "#fff", "color": "#00f" });
    });
});
function isNumber(oNum) {
    if (!oNum)
        return false;
    var strP = /^\d+$/;
    if (!strP.test(oNum))
        return false;
    return true;
}
$(function () {
    $("#btngotopage").click(function () {
        var linkstring = $("#hdlinkstring").val();
        var d = $("#tbgotopage").val();
        var total = $("#hdtotalpages").val();
        if (isNumber(d)) {
            if (d - 1 < 0) {
                $("#lbgotopage").text("输入的数字不能小于1");
                d = 0;
            }
            else if (d - total > 0) {
                $("#lbgotopage").text("不能大于总页数");
                return false;
            }
            else {
                d = d - 1;
                if (linkstring.indexOf("?") > -1)
                    window.location = linkstring + "&p=" + d;
                else
                    window.location = linkstring + "?p=" + d;
            }
        }
        else {
            $("#lbgotopage").text("只能输入数字");
            return false;
        }
    });
});

///------------------设置按照年份过滤-----------------------
$(function () {
    var first = "";
    try {
        first = $.QueryString["yr"];
    }
    catch (err)
    { }
    if (typeof (first) == "undefined")
        first = "";
    var links = $("#navbar a");
    for (var i = 0; i < links.length; i++) {
        var name = $(links[i]).attr("name");
        if (name == first) {
            $(links[i]).attr("class", "active");
            break;
        }
    }
});



//-------------------页面访问统计----------------//
$(function () {
    var url = window.location.href;
    $.post("/Service/statvisit.aspx", { url: url }, function (data, textStatus) {
        //alert(data+"\r\n"+url);
    });
});

//滚动侦测
$(window).scroll(function () {
    var x = $(window).scrollTop();
    if (x > 220)
        $("#returntotop").show();
    else
        $("#returntotop").hide();
});