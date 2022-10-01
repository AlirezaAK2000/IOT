const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <title>Yaghob Barghi</title>
        <style>
.Vending_Machine {
   position: absolute;
   left: 50%;
   top: 50%;
   transform: translate(-50%, -50%);
   width: 15%;
   border: 1px solid black;
   padding: 10px;
   display: flex;
   align-items: center;
   justify-content: center;
}

.order {
  background-color: #FAFBFC;
  border: 1px solid rgba(27, 31, 35, .15);
  border-radius: 6px;
  box-shadow: rgba(27, 31, 35, .1) 0 1px 0;
  box-sizing: border-box;
  cursor: pointer;
  display: inline-block;
  font-size: 14px;
  font-weight: 600;
  line-height: 20px;
  padding: 6px 16px;
  position: relative;
  text-align: center;
  color: #000;
}

.order:hover {
  background-color: #F3F4F6;
}

        </style>
    </head>
    <body>
        <div class="Vending_Machine">
          <form id="main-form">
            <input type="radio" id="Food" name="Food" value="Milk">
            <label for="Milk">Milk | 7000</label><br>
            <input type="radio" id="Food" name="Food" value="Soda">
            <label for="Soda">Soda | 5000</label><br>
            <input type="radio" id="Food" name="Food" value="Peanut Pack">
            <label for="Peanut Pack">Peanut Pack | 4000</label><br>
            <input type="radio" id="Food" name="Food" value="Chips">
            <label for="Chips">Chips | 6000</label><br>
            <br>
            <div class="loc">
              <input class="order" type="submit" value="order" id="submit-button">
            </div>
            <p id="status"></p>
          </form>
        </div>

<script>


document.getElementById('submit-button').onclick = function (e) {
  var form = document.getElementById('main-form');
  var status = document.getElementById('status');
  var data = new FormData(form);
  var items = {};
  for (const entry of data) {
      items[entry[0]] = entry[1];
  }

  var xhr = new XMLHttpRequest();
  xhr.open("POST", '/order', true);

  //Send the proper header information along with the request
  xhr.setRequestHeader("Content-Type", "application/json");

  xhr.onreadystatechange = function() { // Call a function when the state changes.
      if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {
        var respData = JSON.parse(this.responseText) || {};
        var msg = respData["message"];
        status.innerText = msg;
      }
  }
  xhr.send(JSON.stringify(items))

  e.preventDefault();
};


function clear_status(){
  var status = document.getElementById('status');
  status.innerText = "";
}

function check_transaction(){
  var status = document.getElementById('status');
  var xhr = new XMLHttpRequest();
  xhr.open("GET", '/check', true);
  xhr.onreadystatechange = function() { // Call a function when the state changes.
      if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {
        var respData = JSON.parse(this.responseText) || {};
        var msg = respData["message"];
        var is_succeed = respData["succeed"];
        status.innerText = msg;
        if(is_succeed === false){
          setTimeout(clear_status,4000);
        }
      }
  }
  xhr.send(null);
}

setInterval(check_transaction , 1000);
</script>

</body>
</html>

)rawliteral";
