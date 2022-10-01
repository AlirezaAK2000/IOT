const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <title>Alarm</title>
        <style>
          .alarm {
             position: absolute;
             left: 50%;
             top: 50%;
             transform: translate(-50%, -50%);
             width: 60%;
             height: 80%;
             border: 1px solid black;
             padding: 6px;
             display: flex;
             flex-direction: column;
             align-items: center;
             justify-content: center;
          }

          .button {
            background-color: #FAFBFC;
            border: 1px solid rgba(27, 31, 35, .15);
            border-radius: 6px;
            position: relative;
            height: auto;
            width: auto;
            box-shadow: rgba(27, 31, 35, .1) 0 1px 0;
            box-sizing: border-box;
            cursor: pointer;
            display: inline-block;
            font-size: 14px;
            font-weight: 600;
            line-height: 20px;
            padding: 6px 16px;
            text-align: center;
            color: #000;
          }

          .button:hover {
            background-color: #F3F4F6;
          }
        </style>
    </head>
    <body>
        <div class="alarm">
          <div>
            <h1>Alarm</h1>
          </div>
          <div>
            <form action="">
              <p>Time <input type="text" id="time" name="time" "></p><br>
          <div>
            <img src="sound.png" style="width: 5%; height: 5%;">
            <input class="sound" id="sound" name="sound" type="range" min="1" max="100" value="50">
          </div><br>
              <input id="stop" name="stop" class="button" type="button" value="Stop">
              <input id="snooze" name="snooze" class="button" type="button" value="Snooze">
            </form>
          <div>
        </div>

        <script>
          
        document.getElementById('stop').onclick = function() {
            var request = new XMLHttpRequest();
            request.open("GET", "/stop", true);
            request.send();
        }
          


        document.getElementById('snooze').onclick = function() {
            var request = new XMLHttpRequest();
            request.open("GET", "/snooze", true);
            request.send();
        }



        document.getElementById('time').onchange = function(){
            var time = document.getElementById("time").value;
            time = time.split(":");
            var request = new XMLHttpRequest();
            request.open("GET", "/time?h="+time[0] + "&m="+time[1] + "&s="+time[2] , true);
            request.send();
        }


        document.getElementById('sound').onchange = function(){
            var sound = document.getElementById("sound").value;
            var request = new XMLHttpRequest();
            request.open("GET", "/sound?value="+sound, true);
            request.send();
        }
        </script>
    </body>
</html>
)rawliteral";
