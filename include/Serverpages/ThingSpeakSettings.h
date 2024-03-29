#include <avr/pgmspace.h>

const char thingSpeakSettings_html[] PROGMEM =
{
"<!doctype html><html lang=\"en\"><head> <meta charset=\"utf-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\"> <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\"> <link rel=\"stylesheet\" href=\"css/station.css\"> <title>Room Sensor | ThingSpeak Settings</title></head><body> %NAVBAR% <div class=\"container-fluid\" style=\"margin-top:80px\"> <h3>Settings for ThingSpeak reporting</h3> <div class=\"table-responsive-md\"> <table class=\"table\" style=\"background-color: white\"> <thead class=\"thead-light\"> <tr> <th>ThingSpeak</th> </tr></thead> <tbody> <tr> <td> <form action=\"/updateThingSpeak.html\"> <div class=\"form-check form-group\"> <label class=\"form-check-label\" for=\"check1\"> <input type=\"checkbox\" class=\"form-check-input\" id=\"thingSpeakEnabled\" name=\"thingSpeakEnabled\" value=\"Enabled\" %THINGSPEAKENABLED%>Enabled </label> </div><div id=\"thingSpeakForm\"> <div class=\"form-group\"> <label for=\"channelID\">ThingSpeak channel ID:</label> <input type=\"text\" class=\"form-control\" id=\"channelID\" value=\"%THINGSPEAKCHANNELKEY%\" placeholder=\"ThingSpeak channel ID\" name=\"thingSpeakChannel\"> </div><div class=\"form-group\"> <label for=\"thingSpeakApi\">ThingSpeak Api Key:</label> <input type=\"text\" class=\"form-control\" id=\"thingSpeakApiKey\" value=\"%THINGSPEAKAPIKEY%\" placeholder=\"ThingSpeak API Key\" name=\"thingSpeakApiKey\"> </div></div><button type=\"submit\" class=\"btn btn-primary\">Save</button> </form> </td></tr></tbody> </table> </div></div><script src=\"https://code.jquery.com/jquery-3.3.1.slim.min.js\" integrity=\"sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo\" crossorigin=\"anonymous\"></script> <script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js\" integrity=\"sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1\" crossorigin=\"anonymous\"></script> <script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js\" integrity=\"sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM\" crossorigin=\"anonymous\"></script> <script src=\"js/jquery.confirmModal.min.js\"></script> <script src=\"js/thingSpeakSettings.js\"></script></body></html>"
};


const char thingSpeakSettings_js[] PROGMEM =
{

"function init()\n"
"{   \n"
"    if ($('#thingSpeakEnabled').is(\":checked\"))\n"
"    {\n"
"        $(\"#thingSpeakForm\").show();\n"
"    }\n"
"    else\n"
"    {\n"
"        $(\"#thingSpeakForm\").hide();\n"
"    }\n"
"\n"
"    $(\"#thingSpeakEnabled\").click(function()\n"
"    {\n"
"        if($(this).is(\":checked\"))\n"
"        {\n"
"            $(\"#thingSpeakForm\").show();\n"
"        }\n"
"        else\n"
"        {\n"
"            $(\"#thingSpeakForm\").hide();\n"
"        }\n"
"    });\n"
"}\n"
"\n"
"window.addEventListener(\"load\", init, false);"

};