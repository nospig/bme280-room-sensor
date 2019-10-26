var websocket;

function init()
{   
    openWebSocket();
}

function updateSensorReadings(messageData)
{
    var readings = messageData.readings;   
    var currentValue;
   
    $("#sensorReadingsList").empty();

    $("#ReadingsTitle").html('Sensor readings - ' + readings.hostname);

    if(readings.metric)
    {
        currentValue = readings.temp.toFixed(1) + "&#8451";
    }
    else
    {
        var farenheit = (readings.temp * 1.8) + 32;
        currentValue = farenheit.toFixed(1) + "&#8457";
    }
    $("#sensorReadingsList").append('<li class="list-group-item mdi mdi-thermometer">' + 'Temperature: ' + currentValue + '</li>');
    
    currentValue = readings.humidity.toFixed(1) + "%";
    $("#sensorReadingsList").append('<li class="list-group-item mdi mdi-water-percent">' + 'Humidity: ' + currentValue + '</li>');

    if(readings.metric)
    {
        currentValue = readings.pressure.toFixed(1) + " hPA";
    }
    else
    {
        currentValue = readings.pressure.toFixed(1) + " mb";
    }
    $("#sensorReadingsList").append('<li class="list-group-item mdi mdi-gauge">' + 'Pressure: ' + currentValue + '</li>');
}

function openWebSocket() 
{
    websocket = new WebSocket('ws://' + document.location.host + '/ws');
    websocket.onopen = function(evt) { onOpen(evt) };
    websocket.onclose = function(evt) { onClose(evt) };
    websocket.onmessage = function(evt) { onMessage(evt) };
    websocket.onerror = function(evt) { onError(evt) };
}

function onOpen(evt)
{    
    $("#dashboardTitle").html("Dashboard - connected");
}

function onClose(evt)
{
    $("#dashboardTitle").html("Dashboard - not connected");
}

function onMessage(evt)
{
    console.log(evt.data);

    var messageData = JSON.parse(evt.data);

    switch(messageData.type)
    {
        case "sensorReadings":
            updateSensorReadings(messageData);
            break;
    }
}

function onError(evt)
{
}

function doSend(message)
{
    websocket.send(message);
}

window.addEventListener("load", init, false);
