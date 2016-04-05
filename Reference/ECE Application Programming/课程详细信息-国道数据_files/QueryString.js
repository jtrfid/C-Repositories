/// <summary>
/// Get parameters from the URL
/// Author  : Bruno Leonardo Michels
/// Date    : 2010 Apr 21 - 06:12pm GMT+0
/// Version : 0.1
/// Category: jQuery plugin
/// License : Microsoft Public License (MS-PL, http://opensource.org/licenses/ms-pl.html)
/// </sumary>
/// <example>
/// <code>
///     var q0 = $.QueryString["search"]; // ?search=123&cat=1 (123)
///     var q1 = $.QueryString["foo"]; // ?search=123&cat=1 (undefined)
/// </code>
/// </example>
jQuery.extend({'QueryString':window.location.search.length<=1?new Array():function(a){var b=new Array();for(var i=0;i<a.length;++i){var p=a[i].split('=');b[p[0]]=decodeURIComponent(p[1]);}return b;}(window.location.search.substr(1).split('&'))});