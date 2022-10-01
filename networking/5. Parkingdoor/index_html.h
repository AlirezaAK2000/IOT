const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <title>Parking door</title>
        <style>
          body {
            background: url("gray.jpg") no-repeat center center fixed;
            background-size: cover;
            width: 100%;
          }
          .box {
            height: 100px;
            width: 40%;
            background-color: #FAFBFC;
            padding: 1px;
            padding-left: 20px;
            margin: auto;
          }

          .close-message {
            margin-left: 15px;
            color: white;
            font-weight: bold;
            float: right;
            font-size: 22px;
            line-height: 20px;
            cursor: pointer;
            transition: 0.3s;
          }

          .close-message:hover {
            color: black;
          }

          .buttons {
             display: flex;
             flex-direction: column;
             align-items: center;
             gap: 100px;
             justify-content: center;
          }

          .button {
            background-color: #FAFBFC;
            border: 1px solid rgba(27, 31, 35, .15);
            border-radius: 6px;
            position: relative;
            height: 100px;
            width: 40%;
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
      <div class="box">
        <div class="close-message" onclick="this.parentElement.style.display='none';">&times;</div>
        <strong><span id="message"></span></strong>
      </div>
        <div class="buttons">
              <div></div>
              <div></div>
              <input id="open" name="open" class="button" type="button" value="Open">
              <input id="close" name="close" class="button" type="button" value="close">
        </div>

    <script>
    document.getElementById('open').onclick = function() {
        console.log('open button is clicked');
        var request = new XMLHttpRequest();
        request.open("GET", "/open", true);
        request.send();
    }

    document.getElementById('close').onclick = function() {
        console.log('close button is clicked');
        var request = new XMLHttpRequest();
        request.open("GET", "/close", true);
        request.send();
    }

      function updateMessage(){
        var message = document.getElementById('message');
        setInterval(function() {
          getMessage();
        }, 1000);

        function getMessage(){
          var request = new XMLHttpRequest();
          var msg;
          request.onreadystatechange = function(){
            if (request.readyState == 4 && request.status == 200){
              msg = request.responseText;
              console.log(msg);
              message.innerText = msg;
            }
          };
          request.open("GET", "/msg", true)
          request.send();
        }
      }
      updateMessage();
    </script>
    </body>
</html>
)rawliteral";
