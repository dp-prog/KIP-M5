#include "httpd-fsdata.h"	// todo ����������� 18

/* ��� ������ ������� CGI ���������� ���������� ������ shtml

	/* /index.html			- ���� ��������� �������� */
//<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\x0A\
//<!DOCTYPE HTML5>\x0A\
//<meta http-equiv=\"Refresh\" content=\"20;URL=http://%! IP_Device\n/index.shtml/\">\x0A\

static const unsigned char data_index_html[] = "/index.shtml/\x00\
<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\x0A\
<html><head>\x0A\
<title>���-�5</title>\x0A\
</head>\x0A\
<body>\x0A\
<table width=\"900\" cellspacing=\"0\" cellpadding=\"5\" border=\"1\" align=\"center\">\x0A\
<tr><th bgcolor=\"#c0c0c0\"><h1>���-���-�5</h1></th></tr>\x0A\
</table>\x0A\
<table width=\"900\" cellspacing=\"5\" cellpadding=\"5\" border=\"0\" align=\"center\">\x0A\
<tr><td rowspan=\"2\">\x0A\
	<table width=\"400\" cellspacing=\"0\" cellpadding=\"5\" border=\"1\" align=\"center\">\x0A\
		<tr bgcolor=\"#c0c0c0\"><th colspan=\"2\">�������� ���������</th></tr>\x0A\
		<tr><td>�������� ���������� ���, �</td><th>%! V_skz</th></tr>\x0A\
		<tr><td>�������� ��� ���, �</td><th>%! I_skz</th></tr>\x0A\
		<tr><td>�������� ��������� ���, �</td><th>%! V_sum</th></tr>\x0A\
		<tr><td>�������� ��������� ���, �</td><th>%! V_pol</th></tr>\x0A\
		<tr><td>��� �����������, ��</td><th>%! I_pol</th></tr>\x0A\
		<tr><td>���������� ����, �</td><th>%! V_power</th></tr>\x0A\
	</table>\x0A\
	</td><td>\x0A\
	<table width=\"400\" cellspacing=\"0\" cellpadding=\"5\" border=\"1\" align=\"center\">\x0A\
		<tr bgcolor=\"#c0c0c0\"><th colspan=\"2\">��������� �������</th></tr>\x0A\
		<tr><td>��������� ����� ���</td><th>%! door_flag</th></tr>\x0A\
		<tr><td>������� �������� �������</td><th>0000000</th></tr>\x0A\
	</table>\x0A\
</td></tr>\x0A\
<tr><td>\x0A\
	<table width=\"400\" cellspacing=\"0\" cellpadding=\"5\" border=\"1\" align=\"center\">\x0A\
		<tr bgcolor=\"#c0c0c0\"><th colspan=\"2\">�������������� ���������</th></tr>\x0A\
		<tr><td>����������� � ����� ���-�5, ��</td><th>%! T_stm</th></tr>\x0A\
		<tr><td>���������� �������, �</td><th>\x0A\
			<form name=\"V_Out_Form\" method=\"get\" action=\"/index.shtml/\"><input type=\"number\" size=\"5\" name=\"V_out\" value=\"%! V_out\n\" min=\"0\" max=\"100\" step=\"0.01\">\x0A\
					</form></th></tr>\x0A\
	</table>\x0A\
</td></tr>\x0A\
<tr><td>\x0A\
	<table width=\"400\" cellspacing=\"0\" cellpadding=\"5\" border=\"1\" align=\"center\">\x0A\
		<tr bgcolor=\"#c0c0c0\"><th colspan=\"2\">������� ��������������</th></tr>\x0A\
		<tr><td>��������� ����� ��������</td><th>\x0A\
			<form name=\"V_Out_Form\" method=\"get\" action=\"/index.shtml/\"><input type=\"text\" size=\"8\" name=\"Mercury_ID\" value=\"%! Mercury_ID\n\" \">\x0A\
					</form></th></tr>\x0A\
		<tr><td>���������� ����, �</td><th>%! Mercury_V\n\</th></tr>\x0A\
		<tr><td>��������, ��</td><th>%! Mercury_P\n\</th></tr>\x0A\
		<tr><td>����������� ��������, ���</td><th>%! Mercury_sum_P\n\</th></tr>\x0A\
	</table>\x0A\
	</td><td>\x0A\
	<table width=\"400\" cellspacing=\"0\" cellpadding=\"5\" border=\"1\" align=\"center\">\x0A\
		<tr bgcolor=\"#c0c0c0\"><th colspan=\"2\">��������� �����</th></tr>\x0A\
		<tr><td>RS-485 �� [�����: %! RS485_Addr	\x0A]\x0A\
		</td><th>123</th></tr>\x0A\
		<tr><td>RS-485 �������</td><th>%! Mercury_Have\n</th></tr>\x0A\
		<tr><td>Bluetooth</td><th>000000</th></tr>\x0A\
		<tr><td>GSM</td><th>0000</th></tr>\x0A\
	</table>\x0A\
</td></tr>\x0A\
<br>\x0A\
<table width=\"900\" cellspacing=\"0\" cellpadding=\"5\" border=\"1\" align=\"center\">\x0A\
	<tr bgcolor=\"#c0c0c0\"><th colspan=\"4\">��������</th></tr>\x0A\
	<tr><th>http://ngk-ehz.ru</th><th>413124, �.�������, �/� 18</th><th>+7(8453)54-45-15</th><th>info@ngk-ehz.ru</th></tr>\x0A\
</table>\x0A\
	<br><a href=\"http://%! IP_Device\n/index.shtml/\"><center>�������� ��������<center></a>\x0A\
</body>\x0A\
</html>\x0A\
\x00";

	/* /404.html */
static const unsigned char data_404_html[] = "/404.html\x00\
<html>\x0A\
 <body bgcolor=\"white\">\x0A\
 <meta http-equiv=\"Refresh\" content=\"0; URL=index.shtml/\">\x0A\
 </body>\x0A\
</html>\x0A\
\x00";

static const struct httpd_fsdata_file file_index_html[] =
	{{0, data_index_html, data_index_html + 13, sizeof(data_index_html) - 13}};

static const struct httpd_fsdata_file file_404_html[] =
	{{file_index_html, data_404_html, data_404_html + 10, sizeof(data_404_html) - 10}};

	

// ��������� ����
#define HTTPD_FS_ROOT file_404_html
#define HTTPD_FS_NUMFILES 2
