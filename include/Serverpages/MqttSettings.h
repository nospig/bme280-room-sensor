#include <avr/pgmspace.h>

const char mqttSettings_html[] PROGMEM =
{

"<!doctype html><html lang=\"en\"><head> <meta charset=\"utf-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\"> <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\"> <link rel=\"stylesheet\" href=\"css/station.css\"> <title>Room Sensor | MQTT Settings</title></head><body> %NAVBAR% <div class=\"container-fluid\" style=\"margin-top:80px\"> <h3>Settings for MQTT</h3> <div class=\"table-responsive-md\"> <table class=\"table\" style=\"background-color: white\"> <thead class=\"thead-light\"> <tr> <th>MQTT Broker and topics</th> </tr></thead> <tbody> <tr> <td> <form action=\"/updateMqttSettings.html\"> <div class=\"form-check form-group\"> <label class=\"form-check-label\" for=\"check1\"> <input type=\"checkbox\" class=\"form-check-input\" id=\"mqttEnabled\" name=\"mqttEnabled\" value=\"Enabled\" %MQTTENABLED%>Enabled </label> </div><div id=\"mqttForm\"> <div class=\"form-group\"> <label for=\"mqttBroker\">Broker url:</label> <input type=\"text\" class=\"form-control\" id=\"mqttBroker\" value=\"%MQTTBROKERURL%\" placeholder=\"Broker url\" maxlength=\"32\" name=\"mqttBroker\"> </div><div class=\"form-group\"> <label for=\"mqttPort\">Port:</label> <input type=\"number\" class=\"form-control\" id=\"mqttPort\" value=\"%MQTTPORT%\" placeholder=\"Port number\" name=\"mqttPort\"> </div><div class=\"form-group\"> <label for=\"mqttClientId\">Client ID: (leave blank for default)</label> <input type=\"text\" class=\"form-control\" id=\"mqttClientId\" value=\"%MQTTCLIENTID%\" placeholder=\"Client Id\" maxlength=\"64\" name=\"mqttClientId\"> </div><div class=\"form-group\"> <label for=\"mqttUsername\">User name:</label> <input type=\"text\" class=\"form-control\" id=\"mqttUsername\" value=\"%MQTTUSERNAME%\" placeholder=\"User name\" maxlength=\"64\" name=\"mqttUsername\"> </div><div class=\"form-group\"> <label for=\"mqttPassword\">Password:</label> <input type=\"password\" class=\"form-control\" id=\"mqttPassword\" value=\"%MQTTPASSWORD%\" placeholder=\"Password\" maxlength=\"64\" name=\"mqttPassword\"> </div><div class=\"form-group\"> <label for=\"mqttTempTopic\">Temperature topic:</label> <input type=\"text\" class=\"form-control\" id=\"mqttTempTopic\" value=\"%MQTTTEMPTOPIC%\" placeholder=\"Temperature topic\" name=\"mqttTempTopic\"> </div><div class=\"form-group\"> <label for=\"mqttHumidityTopic\">Humidity topic:</label> <input type=\"text\" class=\"form-control\" id=\"mqttHumidityTopic\" value=\"%MQTTHUMIDITYTOPIC%\" placeholder=\"Humidity topic\" name=\"mqttHumidityTopic\"> </div><div class=\"form-group\"> <label for=\"mqttPressureTopic\">Pressure topic:</label> <input type=\"text\" class=\"form-control\" id=\"mqttPressureTopic\" value=\"%MQTTPRESSURETOPIC%\" placeholder=\"Pressure topic\" name=\"mqttPressureTopic\"> </div></div><button type=\"submit\" class=\"btn btn-primary\">Save</button> </form> </td></tr></tbody> </table> </div></div><script src=\"https://code.jquery.com/jquery-3.3.1.slim.min.js\" integrity=\"sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo\" crossorigin=\"anonymous\"></script> <script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js\" integrity=\"sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1\" crossorigin=\"anonymous\"></script> <script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js\" integrity=\"sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM\" crossorigin=\"anonymous\"></script> <script src=\"js/jquery.confirmModal.min.js\"></script> <script src=\"js/mqttSettings.js\"></script></body></html>"

};

const char mqttSettings_js[] PROGMEM =
{

"function init()\n"
"{   \n"
"    if ($('#mqttEnabled').is(\":checked\"))\n"
"    {\n"
"        $(\"#mqttForm\").show();\n"
"    }\n"
"    else\n"
"    {\n"
"        $(\"#mqttForm\").hide();\n"
"    }\n"
"\n"
"    $(\"#mqttEnabled\").click(function()\n"
"    {\n"
"        if($(this).is(\":checked\"))\n"
"        {\n"
"            $(\"#mqttForm\").show();\n"
"        }\n"
"        else\n"
"        {\n"
"            $(\"#mqttForm\").hide();\n"
"        }\n"
"    });\n"
"}\n"
"\n"
"window.addEventListener(\"load\", init, false);"

};