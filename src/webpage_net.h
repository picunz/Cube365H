
// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_selectNetwork[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 Form</title>
<style>
body{ margin: 0;padding: 0;font-family: Arial, Helvetica, sans-serif;background-color: #2c257a;}
.box{ width: 70%; padding: 40px; position: relative; top: 0%; left: 50%; transform: translate(-50%,0%); background-color: #191919; color: white; text-align: center; border-radius: 24px; box-shadow: 0px 1px 32px 0px rgba(0,227,197,0.59);}
h1{font-weight: 500;}
input,select{ border: 0; display: block; background: none; margin: 20px auto; text-align: center; border: 2px solid #4834d4; padding: 14px 10px; width: 70%; outline: none; border-radius: 24px; color: white; font-size: smaller; transition: 0.3s;}
input:focus{ width: 90%; border-color:#22a6b3 ;}
#wifi-picker{ display:none; }
input[type='submit']{ border: 0; display: block; background: none; margin: 20px auto; text-align: center; border: 2px solid #22a6b3; padding: 14px 10px; width: 140px; outline: none; border-radius: 24px; color: white; transition: 0.3s; cursor: pointer;}
input[type='submit']:hover{ background-color: #22a6b3;}

body { font-family: Arial, sans-serif; text-align: center; margin: 20px; }
.btn { display: inline-block; padding: 10px 20px; background: #007bff; color: white; border-radius: 5px; text-decoration: none; }
</style>
</head>
<body>
<form action="/getWifi" class="box" id="my-form">
<h1>Cube 365</h1>
<input name="NET" id="Net" list="wifi-list" value="~CUR_NET~" placeholder="Network name">
<select id="wifi-picker">
   <option value="">Choose detected network</option>
   ~WIFI_LIST~
</select>
<datalist id="wifi-list">
   ~WIFI_LIST~
</datalist>
<div class="part">
<input name="Password" type="password" placeholder="Password">
</div>
<input type="submit" value="Send">
</form>

<form action="/getWifi2" class="box" id="my-form">
<h1>Cube 365</h1>
<input name="NET" id="Net" list="wifi-list" value="~CUR_NET~" placeholder="Network name">
<select id="wifi-picker">
   <option value="">Choose detected network</option>
   ~WIFI_LIST~
</select>
<datalist id="wifi-list">
   ~WIFI_LIST~
</datalist>
<div class="part">
<input name="Password" type="password" placeholder="Password">
</div>
<input type="submit" value="Send">
</form>

<script>
document.addEventListener("DOMContentLoaded", function() {
   const input = document.getElementById("Net");
   const picker = document.getElementById("wifi-picker");

   function showPicker() {
      if (picker.options.length > 1) {
         picker.style.display = "block";
      }
   }

   input.addEventListener("focus", showPicker);
   input.addEventListener("click", showPicker);

   picker.addEventListener("change", function() {
      if (picker.value) {
         input.value = picker.value;
         picker.style.display = "none";
         picker.selectedIndex = 0;
      }
   });
});
</script>
</body></html>
)rawliteral";
