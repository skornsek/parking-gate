// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "YOUR-NAME";
const char* password = "YOUR-PASSWORD";

const char* PARAM_INPUT_1 = "pin";

int RelayPin = 33;
int RampPin = 0000; // Set your custom pin number that will be checked by the server

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <link rel="icon" href="data:,">
    <title>Parking gate</title>
    <style>
        body {
            width: 100%;
            height: 100%;
            padding: 0px;
            margin: 0px;
            font-family: Arial, Helvetica, sans-serif;
        }

        #pinpad{
            margin: 15px;
        }
       

        input[type="password"] {
            width: 100%;
            border: 1px solid rgb(228, 220, 220);
            outline: none;
            font-size: 2rem;
            color: black;
            text-align: center;
            margin-left: -1px;
            margin-right: -1px;
        }

        input:focus {
            outline: none;
        }


        .pw-placeholder{
            width: 90%;
            max-width: 300px;
            text-align: center;
            margin: auto;
            margin-bottom: 15px;
        }

        .grid {
            width: 90%;
            max-width: 300px;
            display: grid;
            gap: 0px;
            grid-template-columns: repeat(3, 1fr);
            margin: auto;
        }

        .pinButton {
            height: 80px;
            border: 0px solid transparent;
            background-color: transparent;
            font-size: 1rem;
            border-radius: 0px;
            color: black;
            -webkit-appearance: none !important;
        }

        .pinButton:hover{
            background-color: #ccc;
        }
        
        #correct, #wrong{
            display: none;
            text-align: center;
        }
    </style>
</head>

<body>
    <div id="pinpad">
        <form>
            <div class="pw-placeholder">
                <input type="password" id="pw" /></br>
            </div>
            <div class="grid">
                <input type="button" value="1" id="1" class="pinButton calc" />
                <input type="button" value="2" id="2" class="pinButton calc" />
                <input type="button" value="3" id="3" class="pinButton calc" />
                <input type="button" value="4" id="4" class="pinButton calc" />
                <input type="button" value="5" id="5" class="pinButton calc" />
                <input type="button" value="6" id="6" class="pinButton calc" />
                <input type="button" value="7" id="7" class="pinButton calc" />
                <input type="button" value="8" id="8" class="pinButton calc" />
                <input type="button" value="9" id="9" class="pinButton calc" />
                <input onclick="clearPw()" type="button" value="clear" id="clear" class="pinButton clear" />
                <input type="button" value="0" id="0 " class="pinButton calc" />
                <input onclick="send()" type="button" value="enter" id="enter" class="pinButton enter" />
            </div>
        </form>
        <p id="correct">
            PIN is correct :) 
        </p>
        <p id="wrong">
            Wrong PIN :(
        </p>
    </div>

</body>
<script>
    function clearPw() {
        var pwInput = document.getElementById("pw");
        pwInput.value = pwInput.value.slice(0, pwInput.value.length - 1);
        console.log(pwInput.value);
    }

    function send() {
        var pwInput = document.getElementById("pw");

        if (pwInput.value.length > 0) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/update?pin=" + pwInput.value, true);

            xhr.responseType = 'text';

            xhr.onload = () => {
                if (xhr.readyState === xhr.DONE) {
                    if (xhr.status === 200) {
                        console.log(xhr.responseText);
                        const box = document.getElementById('correct');
                        box.style.display = 'block';
                        setTimeout(() => {
                            box.style.display = 'none';
                        }, 2000);
                    }
                    if (xhr.status === 400) {
                        console.log(xhr.responseText);
                        const wrongBox = document.getElementById('wrong');
                        wrongBox.style.display = 'block';
                        setTimeout(() => {
                            wrongBox.style.display = 'none';
                        }, 2000);
                    }
                }
            };
            xhr.setRequestHeader('Access-Control-Allow-Headers', '*');
            xhr.setRequestHeader('Access-Control-Allow-Origin', '*');

            xhr.send();
            pwInput.value = "";
        }
    }

    document.addEventListener('click', function (event) {

        /* If the clicked element doesn't have the right selector, bail */
        if (!event.target.matches('.calc')) return;
        /* Don't follow the link */
        event.preventDefault();

        var pwInput = document.getElementById("pw");
        /* Log the clicked element in the console */
        pwInput.value = pwInput.value + event.target.value;

        console.log(pwInput.value);

    }, false);
</script>

</html>
)rawliteral";

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, 1);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/update?pin=<pinNumber>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String pinNumber;
    // GET input1 value on <ESP_IP>/update?pin=<pinNumber>
    if (request->hasParam(PARAM_INPUT_1)) {
      pinNumber = request->getParam(PARAM_INPUT_1)->value();
      if(pinNumber.toInt() == RampPin ){
        Serial.print("Correct PIN: ");
        Serial.print(pinNumber);
        Serial.println();
        request->send(200, "text/plain", "OK");

        // Trigger the rellay for 1 second to simulate button click
        digitalWrite(RelayPin, 0);
        delay(1000);
        digitalWrite(RelayPin, 1);
        
      }else {
        Serial.print("Wrong PIN: ");
        Serial.print(pinNumber);
        Serial.println();
        request->send(400, "text/plain", "NOTOK");
      }
      
    }
    else {
      pinNumber = "No pin sent";
      Serial.print("No PIN: ");
      Serial.print(pinNumber);
      Serial.println();
      request->send(400, "text/plain", "NOTOK");
    }
    

  });

  // Start server
  server.begin();
}

void loop() {
    // Restarting ESP every 20 seconds because my server was chrashing
    // after a few requests (didn't find a better solution for now)
    delay(20000);
    ESP.restart();
    Serial.println("Restarted");
}