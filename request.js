function getXMLHttpRequest()
{
	var xmlhttp = null;
	if (window.XMLHttpRequest)//自动检测当前浏览器的版本，如果是IE5.0以上的高版本的浏览器
	{// code for IE7+, Firefox, Chrome, Opera, Safari
		xmlhttp=new XMLHttpRequest();//创建请求对象
	}
	else////如果浏览器是底版本的
	{// code for IE6, IE5
		xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");//创建请求对象
	}
	return xmlhttp;//返回请求对象
}

function send_data(argument) {

	var xmlhttp =getXMLHttpRequest();

	xmlhttp.onreadystatechange=function()
	{
		if(xmlhttp.readyState==4 && xmlhttp.status==200)
		{
			document.getElementById("msg").innerHTML=xmlhttp.responseText;
		}
	}
	var ip_info;
	var path = "/cgi-bin/a.cgi?";
	switch(argument)
	{
		//document.getElementById("msg").value = "";   
		case 1:
			ip_info = document.getElementById("ip").value;
			path = path + "setip:" + ip_info;
			break;
		case 2:
			ip_info = document.getElementById("ip").value;
			path = path + "delip:" + ip_info;
			break;
		case 3:
			path = path + "help";
			break;
		case 4:
			path = path + "saveset";
			break;
		case 5:
			path = path + "showip";
			break;
		case 6:
			path = path + "showarp";
		case 7:
			path = path + "setip_getway" + ip_info;
			break;
		default:
			break;

	}
	xmlhttp.open("GET", path, true);
	xmlhttp.setRequestHeader("If-Modified-Since", "0");

	xmlhttp.send();
}