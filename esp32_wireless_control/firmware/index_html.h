#ifndef INDEX_HTML_H
#define INDEX_HTML_H

/* clang-format off */
const char html_content[] = R"=====(
  <DOCTYPE html> 
  <html lang="en"> 
  <head> 
  <title>%STR_TITLE%</title> 
  <meta charset='utf-8'> 
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <style> 
    body { 
      background-color: #000000; 
      text-align: center; 
      font-family: system-ui; 
      display: flex;
      padding: 10px; 
      justify-content: center; 
    } 
    
    button { 
      background-color: #FF0000; 
      color: black; 
      border: none; 
      padding: 10px 10px; 
      border-radius: 40px; 
      text-align: center; 
      text-decoration: none; 
      display: inline-block; 
      font-size: 14px; 
      font-family: system-ui; 
      margin-top: 10px; 
      cursor: pointer; 
      width: 50%; 
      min-width: 120px; 
      font-weight: bold; 
    }
    button:disabled, button[disabled]{
      background-color: #fa6b6b;
      color: #262626;
    } 
    select { 
      font-size: 16px; 
      padding: 5px; 
      color: red; 
      background-color: #000000; 
      border: 2px solid #ff0000; 
      border-radius: 8px; 
      font-family: system-ui; 
    }
    input[type='number'] { 
      font-size: 16px; 
      background-color: #000000; 
      padding: 5px; 
      font-family: system-ui; 
      border: 2px solid #ff0000; 
      color: red; 
      border-radius: 8px; 
      box-sizing: border-box; 
    } 
    input::placeholder {
      color: red;
    }
                              
    label { 
      display: inline-block; 
      text-align: left; 
      margin: 10px; 
      font-size: 20px; 
    }
    
    h1 { 
      font-size: 26px; 
      font-family: system-ui; 
      color: red; 
    } 
    h2 { 
      font-size: 24px; 
      font-family: system-ui; 
      color: red; 
    } 
    h3 { 
      font-size: 16px; 
      font-family: system-ui; 
      color: #FF6B00; 
      } 
    h4 { 
      font-size: 12px; 
      font-family: system-ui; 
      color: #FF6B00; 
    } 
    summary { 
      font-weight: bold; 
      font-family: system-ui; 
      font-size: 16px; 
      padding-top: 20px; 
      color: #FF6B00; 
    } 
    .content { 
      max-width: 450px; 
      width: 100%; 
      background-color: #000000; 
    } 
    .card { 
      background-color: #000000; 
      padding: 1px 16px 16px 16px; 
      border-radius: 32px; 
      margin-top: 20px; 
      box-shadow: 0em 0em 1em rgb(255 0 0 / 100%); 
    } 
    .grid2 { 
      display: flex; 
      flex-direction: row; 
      flex-wrap: wrap; 
      color: #FF6B00; 
      justify-content: center; 
      align-items: center; 
    } 
    .grid2 > h3 { 
      width: 50%; 
      min-width: 120px; 
    } 
    .grid2 > select, 
    .grid2 > input { 
      width: 50%; 
      min-width: 120px; 
      height: 40px; 
    } 
    .grid2 > .checkbox { 
      width: 50%; 
      min-width: 120px; 
      height: 20px; 
      margin: 0px; 
    } 
    .header { 
      display: flex; 
      justify-content: center; 
    }
    .button-group { 
      padding-top: 10px; 
      display: flex; 
      flex-wrap: wrap; 
      justify-content: center; 
    } 
    .right-separator { 
      border-right: 2px solid #000000; 
      -webkit-user-select: none; /* Safari, Chrome */ 
      -moz-user-select: none; /* Firefox */ 
      -ms-user-select: none; /* Internet Explorer/Edge */ 
      user-select: none; /* Standard property */ 
      -webkit-touch-callout: none; /* Disable callout menu on iOS */ 
    } 
    .left-separator { 
      border-left: 2px solid #000000; 
      -webkit-user-select: none; /* Safari, Chrome */ 
      -moz-user-select: none; /* Firefox */ 
      -ms-user-select: none; /* Internet Explorer/Edge */ 
      user-select: none; /* Standard property */ 
      -webkit-touch-callout: none; /* Disable callout menu on iOS */ 
    } 
    #logo { 
      background-image: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAJYAAAB5CAYAAADbE9i1AAAACXBIWXMAAAsTAAALEwEAmpwYAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAA4KSURBVHgB7V3dddu4Eh7l5P2qA3MrWG0FYiqwtwJqn+6jkwqoVKDs2z33hd4KlFRAuQI7FZB52zd7K5jlCEAI0RQJkuAAsPWdM6Zsk8IHYDAzxO8CAgQCRNVlWclK/unqzK0/KnmW8rgQV2fQeKurCe+y4l1CYFiAx8BaeX6V1xhEpYzFUcFAVBRd7xfiah0o+K5BXEkiEPkZC8X7UMn3hbheYApq1ZXcVpJX8lQJziyUxr6SBCcoLTUCZt4o07rFaY3t9UJWylYWFDqW3FTJGsrkmvcDTmwcrwZVIcSeVEqb7Ht4k5XjskyDuaMIG94WqkxvPFYokhRb4iHPG0KbFJUk8NpRZfJGZtbXisixxZUEqFBvQ8ECqZjbFt5R4ArVFIrDIggdKILbL54XdoF1f5jOnV4mfI2hpgrlbUrXhzugsFKF5wWcNwuYlAxFy/aZtw0psKVBeQ3030qR7Fp4f8TXa6XOyRZ8B4YTk6QN3uSy7wLgPZfk6GvshcKFFAEUYtLgHQXCe24pvFMuFN0IIbiQpME7lMbAJVSHfsRdKDo7Qyi02wbvFb69eMpUEnCJgJQqbfC+KJWvynVRqlcvD8ANFDFVcIWDIlC/KFW/5MgdyAdUOYVeOHh5+zMRqtdb4EZglbPSeC8D4u1KcnTV3YDhDHWkDd5ZILxdSK+VQmFQbmAOoBi4DKGgigbvj4HwdiE59lgpPB3m2oJNYDjBOuLLuCoU3tyS9tT5ueG5GGwAw4qrdg3uofDmFApnVj113jUYX6CNKTcYTnxCGY403qG4bk7ZdSkFmk8i2MEUoOhM9L2wlCSNAgqFN4cUKNY4dtX10ClDcdf3vYdu7MFfHFcJg1jISauc/9L+l4LfKKFegEqiVmgvoV4pTbKC6W7nSyWfz60CR5FOBsNjJyrjAwwF+jlkk6NYx7fq4B17yFvnPkhRUHiNMWsXixmsVFMSGAr0J/CljKemFeIR717uKDpuSXFiw7yRi88M8mgrlupVXhgC9MNaDVIoj3gr7kkLP7Vyum2xawyGkIqRnkl33fOs7enXiSlvH1p9jiOGF9CPadEpvlygcWPALYeBwFPLs0ceK9WUAkyAblv96EFQdB9bFdiI/WRZFgO+I4YRwGn9UjYkBgOSrsYDC5wwLRbd9redxDQolHxMOU7rH3pZJnNZqabkfURc9VsVOG0LIZf9VmmDy5Slb09oaSEp8i9lW3WRyRiJWFEqyduV+040DqTcNqz9pPlQ6G4JXtpFqmAmU0xVKsnbhfu+bVRmYel7cxgJdLvg9ukcKRfB7+SlRujGDaaN9AvL3z+0E9WVlWpK3EYuYyaRggUgvxvcNyq0mCENY3eIfm0LsGsjWDASKMASkLelFng6g6KYKZ3cIN8+bl5SNElyu5MILIGZtx5XzbnxyVNPnrczpj1VljpRTneSgSUgb1xYMKcbt+Q3hC2WEuL6TnJeAx8+gz38Cnw4aJ8zmB8neUMx35zWSfq+p9WR33v9FwZ8tXzKQgx8uKMfKFarRDA/VjI9umYQziZpMf1QFouL9B3YRQQ8eK4axL38zGXdY3lNIayd9yL68Q75SFPlfAO74OL+qH3m2rszktdP4PgMoIGgqUFXZLEi4MEBLAJ5W/EjOABVkAwdbMalHFiRYl0BD76CXUTAh4P2mdN6HBvPQsxbd6LcI7HktFjfwS64GgSBjnmLQLhB2/nogu52/4BwEJFiscQMMxzfxhXrPEvuMYg+Gk7LEakPksMWwkDEZbHmqAwuxVIu/LqSeFEv23KBPyGQQzHfAQ/miEs4FIt4q8BZHcQJwBdMR/ovcm1gCC4xClmx5kZZyQf5VhZDHWOR1boDR25Jnqz6BTzHRbFOQTwPlXzcCqWiztA9nK4IP/5OCkf3gFCyEnjxGTx3iX1L7N8SSimP/wX47f/VwDy095WR1aJhnZuqdsu/K0X8n1DGFTD1rZFLROESc/AV6MMqjnG8s5k5P6TCBW5A8Ecp9Pm2Uqo1w0yDrKcMdjOnP76+mRTL+tbOyDPjtcB6gw6lWGpwuGBIf9dTBkP3Vs0ZOB/ToRirhPkRgX2UMD8iEFaj1OQRhRWLYH50xqYD3hJLEC8dH4AHJVfwfhyYBLvgeiGgSX0UV1F/1kH+LQEe9OZRviV2DZfRG+RvNDsDGSccUPBeAg8oUz/AHv4BPpAi0cyM/8jfY+BBaXgfWa0YTvv2yko22nQfQgQ8OFosm5XdhRjsgnNoZSXTo1YfAx+M6mZx2pFL+GmlGrdeAw9KslhcFUSv6J/AHkrgA1kC5ZY4p0OXpjcuxOIOsqiHFoVSiIEHP9SbBTJJDBaBfOvpci3NjCnNJ7AI5F14sny3qPfy5IBtU8zpDhVi4IHtvCXAA9oP9lm9FXJV0AbtDh5z8b6jH1iPGXLgAJaAgvMGeFDSD6VYB+ABKZXN06XuYX6U2o7MXK2ecAB72AAfDj8/Mftfm3tAccSHa5kW92pxW2U01/4S52TVJMC5sYS1netw3vG6lCmdpuRgCci72UvRRmDPSIAkBgvA+QZi9V1luE8Qs9LwkH8nnswHEgVaMPc4jxv/yQ3d7L81eegF3RyulbQRWTKTIMnBAtCuGy9Qvvk5qpwCLADdbB6yPEcmd0Amg4lAe+4wx9pSLR1VzmQ3WH3HnQPeWRehGPkJkWxhAizx3mnf50qpSCKYAEdKRXLdR8zVtoMZToi5cLy1pfzeaN8TOVSqHEYCRWNwpVSFCcHUEbkjQRzZYnHcy0dz4/8Vuj3uJYERQLeNwYw3Cs13uVlqMaaAB/B+kgoVNZ7fOszzMd8wAujH5raRKVkfJunvcaD1wm5rm2PLeYE4/ngS25LAAEjeuQe8syGkXfTdnCWOhv06WAfdhVRMaiDX2BK7eVQxJAUYwjPeJBEMAbqNtdqEFCbBCW9NKBrMFv073T4x4D3mlNW55WzXyKIjM9TKC+DbfGMIHjWhbYVoThntClNifa4ygSzdFdT7LkTgH2j2xC/0QeMeBG+otyAYBuQfI3uLcq2VdxEQ7wSmAP3fVzxkybRydv1WOkSKPr1Z9N2AwhTncIFtlCBdCQpXR2UcQRj4pc8F9i5YDWXbnACx1SonhXCUaruwuUYCLy5xLhc4ZsTAlRjvwdHrCrUCiEB8sY9viSGhBLGY9BnDcoElDHgLNN67QX7h73DBFFC3yIdFvfg1hXBc4MaqC2wC/es4DUn0WRQhvQWmwAH0d8OvICoHxSyK4HizAN3N/QldqVxMdx4r1lZTDcJFuV61UmXgEhflMlYql9Odw1IqBbzEXK9JqVLwCXh5W1RCszkTrVwuSjUVKHqRXU+TdSkFapMSMZyY6mRBiZcIqDBtS45aZ2dA5fCA4XTSwluLu24beQ/Fcp+sUgoGKOZnFwEU8FjJsTEfH8VeoL7zLpDv0PR5gCJ4TQMo7CFC1qhppVyv7TPlneJrmkggCz7zvOBN5IX7QD/W9vXJ4OV0QSFgBcvw5SJX35ZitQnxW8NbQSAKdtZ1VH+78Zz37k0pVBNSwegtyqf4JMeWVdMt3Dfol8Uy4v3mgPXCTBdKNrpStMaRO+Kdov1DsCbBeGoyN1DEM3S8SAynB33bAM3gVAtev1byfWHpNDGpmGuoeZPYsiDqsIcDCO7fbPG2DW8Vqw1YrxCONFGVFjVuL7WrqhASOjmB62CqIyRvOueGrhF0836Gmq9T3hdccMEFF1xwwQUXnLydTt4n3ilQjANuMeChAhS95ym8AmC9qjrs/GDdc76GQCH5F/AK4JNivVcfsH4NJjzO1T8yJR3ZRxTR54WbQzB1HuRuyr7VwY389t7f8exsddJIU+VNbQgz9HnRbyf9cluP8bbj4fhML/BVV4LYPjdrC91EY62HubBlYbB9Kk/
ecb+yBpnGI+u4f3mmXHvX6sln9yPK6oXFwnrF9QP2D1E155JRPj/2PBNp9fMzv6D9chy0RLEZbNGlKPLLUqyHXjLsmeejfWcq09FnWXY9pyvxk0zLxrEgMdbKpQabk477N40CJx63HfdnjXJda+V13cNNVfBe1odeJ7EBx7TxPdkApVIbAq8N04xa6mcPWLeMj9rNZF0S7AnK0TDGki0waRYo1tOXk45nY60yI7AM9d0G9200Hr1DNLJyksbf1IyIrOO5qI0T1lN0bg04UiO560vLIM2f1sjgWdTrh2KsP0H4xZ3U9AOI8bN7W7uLyC17voFQLjoHUMVKQ8bQHmfd7cQcdyaxTnXPN6lIVLERmDcKdd9j4/sOYB7zkJGgsqUy+8Pg/khel2eUyKT74qDXz3siXH3ZBxDmbi2vNyC2MPzdYpBMe2tFcFpAMZgPLns52HoOKHZBJOtyAFGGdI2g/3xm1dim5PeoVCA8z66qw08G96s0y8b/7mAMpNlszt/emZjRAa6w1QVozycdz8amJn0MRrjC1ODe5RnXshngCvPG39XLz86AY4anC2VNA/CHM98ZGzx7wpc2XqOHKHDbyBtJe9WsB9NWE2O35WmbgUmZ3YB7UB6XUoFt9Vir/C5V3CHLp1cpF/W0GOKzlc8uod6kzcSDlNJd00Z5dCVDcWOQJinvVirl8TQxEI1gDUOB57sBeoNlPH1jKzruW2ppPGmy98BipVoeuiyJscWS99/h6duSsiQmniA6Uydpz3Nt3Q2xxiEakaYJ1xcWa6H+CWJSHbVY5WfvTYJU+exVJf/0xWMo3gqPHYsggvmlfPbsRDvZWonbj7mCd6xb5Nk0tHwa89DyqyYW0ikaxnlpPl89c99zf9TGEev5YL3pjkhT1c9J/f8LwULVu5PReR4AAAAASUVORK5CYII='); 
      width: 150px; 
      height: 120px; 
      margin-top: 10px; 
    } 
    #status { 
      font-size: 24px; 
      margin: 0px; 
      font-family: system-ui; 
    } 
  </style> 
  <script> 
    var lastClick = 0; 
    var delay = 500;
    var currentLangIndex = 0; 

    function sendRequest(url) { 
      var xhr = new XMLHttpRequest(); 
      xhr.onreadystatechange = function () { 
        if (this.readyState == 4 && this.status == 200) { 
          document.getElementById('status').innerHTML = this.responseText; 
        } 
      }; 
      xhr.open('GET', url, true); 
      xhr.send(); 
    } 

    setInterval(function () { 
      sendRequest('/status'); 
    }, 5000); 

    function requestFirmwareVersion() {
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById('firmware').innerHTML = '%STR_FIRMWARE_VERSION%: ' + this.responseText;
        }
      };
      xhr.open('GET', '/version', true);
      xhr.send();
    }

    function changeLanguage() {
      var lang = document.getElementById('language-select').value;
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          window.location.reload();
        }
      };
      xhr.open('GET', '/setlang?lang=' + lang, true);
      xhr.send();
    }
                          
    function sendStartRequest(url) { 
      var direction = document.getElementById('hemisphere-select').value; 
      var trackingSpeed = document.getElementById('tracking-speed-select').value; 
      var starturl = url + '?direction=' + direction + '&trackingSpeed=' + trackingSpeed; 
      sendRequest(starturl); 
    } 

    function sendSlewStop() { 
      lastClick = Date.now(); 
      sendRequest('/stopslew'); 
    } 

    function sendSlewRequest(directionArg) { 
      if (lastClick >= (Date.now() - delay)) 
        return; 
      lastClick = Date.now();
      if (directionArg == 'left') {
        direction = 0;
      } else if (directionArg == 'right') {
         direction = 1;
      }
      var index = document.getElementById('slew-select').selectedIndex; 
      if (index == 6) { 
        var speed = document.getElementById('custom-speed').value; 
      } else { 
        var speed = document.getElementById('slew-select').value; 
      } 
      var slewurl = '/startslew?speed=' + speed + '&direction=' + direction; 
      sendRequest(slewurl); 
    } 

    function checkCustom() { 
      var index = document.getElementById('slew-select').selectedIndex; 
      if (index == 6) { 
        document.getElementById('custom-speed-text').style.display='block'; 
        document.getElementById('custom-speed').style.display='flex'; 
      } else { 
        document.getElementById('custom-speed-text').style.display='none'; 
        document.getElementById('custom-speed').style.display='none'; 
      } 
    } 

    function chooseDither() {
      if (document.getElementById('dither').selectedIndex == 1) {						
        document.getElementById('ditherFreqDiv').style.display='flex'; 
     		document.getElementById('ditherFocalLengthDiv').style.display='flex'; 
        document.getElementById('ditherPixelSizeDiv').style.display='flex';
			} else {
			  document.getElementById('ditherFreqDiv').style.display='none'; 
        document.getElementById('ditherFocalLengthDiv').style.display='none'; 
        document.getElementById('ditherPixelSizeDiv').style.display='none';
      }
		}

    function changeMode() {
      calculateCaptureTime();
		  var mode = document.getElementById('mode').selectedIndex; 
      switch (mode) {
        case 0:
          document.getElementById('exposureDiv').style.display='flex'; 
          document.getElementById('framesDiv').style.display='none'; 
          document.getElementById('panAngleDiv').style.display='none'; 
          document.getElementById('panDirectionDiv').style.display='none'; 
          document.getElementById('trackEndDiv').style.display='flex'; 
          document.getElementById('ditherChoiceDiv').style.display='flex'; 
          chooseDither();
          break;
        case 1:
          document.getElementById('exposureDiv').style.display='flex'; 
          document.getElementById('framesDiv').style.display='flex'; 
          document.getElementById('panAngleDiv').style.display='none'; 
          document.getElementById('panDirectionDiv').style.display='none'; 
          document.getElementById('trackEndDiv').style.display='flex'; 
          document.getElementById('ditherChoiceDiv').style.display='flex'; 
          chooseDither();
          break;
        case 2:
          document.getElementById('exposureDiv').style.display='none'; 
          document.getElementById('framesDiv').style.display='none'; 
          document.getElementById('panAngleDiv').style.display='none'; 
          document.getElementById('panDirectionDiv').style.display='none'; 
          document.getElementById('trackEndDiv').style.display='none'; 
          document.getElementById('ditherChoiceDiv').style.display='none';
          document.getElementById('dither').value = 0;
				  document.getElementById('dither').dispatchEvent(new Event('change'));
				  break;
        case 3:
          document.getElementById('exposureDiv').style.display='none'; 
          document.getElementById('framesDiv').style.display='none'; 
          document.getElementById('panAngleDiv').style.display='flex'; 
          document.getElementById('panDirectionDiv').style.display='flex'; 
          document.getElementById('trackEndDiv').style.display='none'; 
          document.getElementById('ditherChoiceDiv').style.display='none'; 
          document.getElementById('dither').value = 0;
				  document.getElementById('dither').dispatchEvent(new Event('change'));
				  break;
          default:
      }

    }

		function calculateCaptureTime() {
      exposureTime = document.getElementById('mode').selectedIndex < 2 ? document.getElementById('exposureTime').valueAsNumber : 1;
      exposures = document.getElementById('exposures').valueAsNumber; 
			predelay = document.getElementById('preDelay').valueAsNumber; 
      delay = document.getElementById('delay').valueAsNumber;
      frames = document.getElementById('mode').selectedIndex == 1 ? document.getElementById('frames').valueAsNumber : 1;
      panAngle = document.getElementById('mode').selectedIndex == 3 ? document.getElementById('panAngle').valueAsNumber : 1;
      ditherFrequency = document.getElementById('mode').selectedIndex < 2 && document.getElementById('dither').selectedIndex == 1 ? document.getElementById('ditherFrequency').valueAsNumber : 1;
      focalLength = document.getElementById('mode').selectedIndex < 2 && document.getElementById('dither').selectedIndex == 1 ? document.getElementById('focalLength').valueAsNumber : 1;
      pixelSize = document.getElementById('mode').selectedIndex < 2 && document.getElementById('dither').selectedIndex == 1 ? document.getElementById('pixelSize').valueAsNumber : 1;
      total = exposureTime+exposures+predelay+delay+frames+panAngle+ditherFrequency+focalLength+pixelSize;
 			captureTime  = (((delay + exposureTime)*exposures-delay)*frames) + predelay;
      if (isNaN(captureTime)) {
        document.getElementById('captureTime').innerHTML = 'Capture Time: 0 h, 0 m, 0 s';
      } else {
        hours = Math.floor(captureTime/3600);
        minutes = Math.floor((captureTime - 3600*hours)/60);
        seconds = Math.floor(captureTime - 3600*hours - minutes*60);
        document.getElementById('captureTime').innerHTML = '%STR_CAPTURE_TIME%: ' + hours + ' h, ' + minutes + ' m, ' + seconds + ' s';
      }
      saveButton = document.getElementById('save-button');
      captureButton = document.getElementById('capture-button');
      if (isNaN(total)) {
        saveButton.disabled = true;
        captureButton.disabled = true;
      } else {
        saveButton.disabled = false;
        captureButton.disabled = false;
      }
    }

    function handleLoad() {
      requestFirmwareVersion();
      changeMode();
    }

    function sendCaptureInformation(mode) {
      var preset = document.getElementById('preset-select').selectedIndex;
      var captureMode = document.getElementById('mode').selectedIndex;
      switch (captureMode) {
        case 0:
          var exposureTime = document.getElementById('exposureTime').value.trim(); 
          var frames = "1";
          var panAngle = "1";
          var panDirection = "1";
          var enableTracking = document.getElementById('enableTracking').selectedIndex;
          var ditherChoice = document.getElementById('dither').selectedIndex;
          if (ditherChoice == 1){
            var ditherFrequency = document.getElementById('ditherFrequency').value.trim();
            var focalLength = document.getElementById('focalLength').value.trim(); 
            var pixelSize = Math.floor(parseFloat(document.getElementById('pixelSize').value.trim()) * 100); 
          } else {
            var ditherFrequency = "1";
            var focalLength = "1";
            var pixelSize = "1";
          }
          break;
        case 1:
          var exposureTime = document.getElementById('exposureTime').value.trim(); 
          var frames = document.getElementById('frames').value.trim();
          var panAngle = "1";
          var panDirection = "1";
          var enableTracking = document.getElementById('enableTracking').selectedIndex;
          var ditherChoice = document.getElementById('dither').selectedIndex;
          if (ditherChoice == 1){
            var ditherFrequency = document.getElementById('ditherFrequency').value.trim();
            var focalLength = document.getElementById('focalLength').value.trim(); 
            var pixelSize = Math.floor(parseFloat(document.getElementById('pixelSize').value.trim()) * 100); 
          } else {
            var ditherFrequency = "1";
            var focalLength = "1";
            var pixelSize = "1";
          }
          break;
        case 2:
          var exposureTime = "1"; 
          var frames = "1";
          var panAngle = "1";
          var panDirection = "1";
          var enableTracking = 0;
          var ditherChoice = 0;
          var ditherFrequency = "1";
          var focalLength = "1";
          var pixelSize = "1";
				  break;
        case 3:
          var exposureTime = "1"; 
          var frames = "1";
          var panAngle = Math.floor(parseFloat(document.getElementById('panAngle').value.trim()) * 100);
          var panDirection = document.getElementById('panDirection').selectedIndex;
          var enableTracking = 0;
          var ditherChoice = 0;
          var ditherFrequency = 1;
          var focalLength = 1;
          var pixelSize = 1;
          break;
          default:
      }
      var exposures = document.getElementById('exposures').value.trim();
      var preDelay = document.getElementById('preDelay').value.trim();
      var delay = document.getElementById('delay').value.trim();

      var intervalometerUrl = '/setCurrent?mode=' + mode +'&preset=' + preset + '&captureMode=' + captureMode + '&exposureTime=' + exposureTime + '&exposures=' + exposures +'&preDelay=' + preDelay + '&delay=' + delay + '&frames=' + frames + '&panAngle=' + panAngle + '&panDirection=' + panDirection + '&enableTracking=' + enableTracking + '&ditherChoice=' + ditherChoice + '&ditherFrequency=' + ditherFrequency + '&focalLength=' + focalLength + '&pixelSize=' + pixelSize;
      sendRequest(intervalometerUrl); 
    }

    function sendPresetReadRequest() {
      var preset = document.getElementById('preset-select').selectedIndex;
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          var settings = JSON.parse(this.responseText);
           document.getElementById('mode').selectedIndex = settings.mode;
           document.getElementById('exposures').value = settings.exposures;
           document.getElementById('delay').value = settings.delay;
           document.getElementById('preDelay').value = settings.preDelay;
           document.getElementById('exposureTime').value = settings.exposureTime;
           document.getElementById('panAngle').value = settings.panAngle/100;
           document.getElementById('panDirection').selectedIndex = settings.panDirection;
           document.getElementById('dither').selectedIndex = settings.ditherChoice;
           document.getElementById('ditherFrequency').value = settings.ditherFrequency;
           document.getElementById('enableTracking').selectedIndex = settings.enableTracking;
           document.getElementById('frames').value = settings.frames;
           document.getElementById('pixelSize').value = settings.pixelSize/100;
           document.getElementById('focalLength').value = settings.focalLength;
           document.getElementById('panDirection').dispatchEvent(new Event('change'));
           document.getElementById('mode').dispatchEvent(new Event('change'));
           document.getElementById('dither').dispatchEvent(new Event('change'));
           document.getElementById('enableTracking').dispatchEvent(new Event('change'));


        }
      };
      var html = '/readPreset?preset=' + preset;
      xhr.open('GET', html, true);
      xhr.send();

    }


  </script> 
  </head> 
  <body onload="handleLoad()">
    <div class="content"> 
      <div class="header"> 
        <div id="logo"></div> 
      </div> 
      <h1>%STR_TRACKER_CONTROLLER%</h1>
      <div class="language-selector">
            <select id="language-select" onchange="changeLanguage()">
              %LANG_SELECT%
            </select>
      </div> 
      <div class="card">
        <h2>%STR_TRACKING%</h2>
        <div class="grid2"> 
          <h3>%STR_HEMISPHERE%:</h3> 
          <select aria-label="Hemisphere" id='hemisphere-select'> 
            <option value='1'>%STR_NORTH%</option> 
            <option value='0'>%STR_SOUTH%</option> 
          </select><br> 
        </div> 
        <div class="grid2"> 
          <h3>%STR_TRACKING_RATE%:</h3> 
          <select aria-label="tspeed" id='tracking-speed-select'> 
            <option value='0'>%STR_SIDEREAL%</option> 
            <option value='1'>%STR_SOLAR%</option> 
            <option value='2'>%STR_LUNAR%</option> 
          </select><br> 
        </div> 
        <div class="button-group"> 
          <button class="right-separator" type="button" onclick="sendStartRequest('/on')">%STR_BTN_ON%</button> 
          <button class="left-separator" type="button" onclick="sendRequest('/off')">%STR_BTN_OFF%</button> 
        </div> 
      </div> 
      <div class="card"> 
        <h2>%STR_SLEW_CONTROL%</h2> 
        <div class="grid2"> 
          <h3>%STR_SPEED_MULTIPLIER%:</h3> 
          <select aria-label="slew" id='slew-select' onchange="checkCustom();"> 
            <option value='2'>2 x %STR_TRACKING_RATE%</option> 
            <option value='8'>8</option> 
            <option value='50'>50</option> 
            <option value='100'>100</option> 
            <option value='200'>200</option> 
            <option value='400'>400</option> 
            <option value='custom'>%STR_CUSTOM%</option> 
          </select> 
        </div> 
        <div class="grid2"> 
          <h3 id='custom-speed-text' style="display:none; align-items:center;">%STR_CUSTOM_SPEED%:</h3> 
          <input id='custom-speed' type='number' value='2' min='2' max='400' style="display:none;"/> 
          <br> 
        </div> 
        <h4>%STR_SLEW_HINT%</h4> 
        <div class="button-group"> 
          <button class="right-separator" type="button" ontouchstart="sendSlewRequest('left')" onmousedown="sendSlewRequest('left')" ontouchend="sendSlewStop()" onmouseup="sendSlewStop()" onmouseleave="sendSlewStop()" ontouchcancel="sendSlewStop()">%STR_SLEW_LEFT%</button> 
          <button class="left-separator" type="button" ontouchstart="sendSlewRequest('right')" onmousedown="sendSlewRequest('right')" ontouchend="sendSlewStop()" onmouseup="sendSlewStop()" onmouseleave="sendSlewStop()" ontouchcancel="sendSlewStop()">%STR_SLEW_RIGHT%</button> 
        </div> 
        <button type="button" onclick="sendSlewStop()" >%STR_ABORT_SLEW%</button> 
      </div> 
      <div class="card"> 
        <h2>%STR_INTERVALOMETER%</h2>
		    <div class="grid2">
          <h3>%STR_PRESET%:</h3> 
          <select aria-label="preset" id='preset-select' > 
            <option value='0'>%STR_PRESET% 1</option> 
            <option value='1'>%STR_PRESET% 2</option> 
            <option value='2'>%STR_PRESET% 3</option> 
            <option value='3'>%STR_PRESET% 4</option> 
		        <option value='4'>%STR_PRESET% 5</option>
            <option value='5'>%STR_PRESET% 6</option>
            <option value='6'>%STR_PRESET% 7</option>
            <option value='7'>%STR_PRESET% 8</option>
            <option value='8'>%STR_PRESET% 9</option>
            <option value='9'>%STR_PRESET% 10</option>
          </select> 
        </div> 
		    <div class="button-group"> 
          <button class="right-separator" type="button" id='save-button' onclick="sendCaptureInformation('save');" disabled>%STR_SAVE_PRESET%</button> 
          <button class="left-separator" type="button" onclick="sendPresetReadRequest()">%STR_LOAD_PRESET%</button> 
        </div> 
        <br>
        <div class="grid2">
          <h3>%STR_MODE%:</h3> 
          <select aria-label="mode" id='mode' onchange="changeMode();"> 
            <option value='0'>%STR_LONG_EXPOSURE_STILL%</option> 
            <option value='1'>%STR_LONG_EXPOSURE_MOVIE%</option> 
            <option value='2'>%STR_DAY_TIME_LAPSE%</option> 
            <option value='3'>%STR_DAY_TIME_LAPSE_PAN%</option> 
          </select> 
        </div> 
        <div class="grid2" id='exposureDiv'> 
          <h3>%STR_EXPOSURE_LENGTH%:</h3> 
          <input type='number' id='exposureTime' placeholder='%STR_EXPOSURE_HINT%' onchange="calculateCaptureTime();"> 
        </div> 
        <div class="grid2"> 
          <h3>%STR_NO_EXPOSURES%:</h3> 
          <input type='number' id='exposures' placeholder='%STR_NUM_EXPOSURES_HINT%' onchange="calculateCaptureTime();"> 
        </div> 
        <div class="grid2"> 
          <h3>%STR_PRE_DELAY%:</h3> 
          <input type='number' id='preDelay' placeholder='%STR_EXPOSURE_HINT%' onchange="calculateCaptureTime();"> 
        </div> 
        <div class="grid2"> 
          <h3>%STR_DELAY%:</h3> 
          <input type='number' id='delay' placeholder='%STR_EXPOSURE_HINT%' onchange="calculateCaptureTime();"> 
        </div> 
        <div class="grid2" id='framesDiv'> 
          <h3>%STR_FRAMES%:</h3> 
          <input type='number' id='frames' placeholder='%STR_NO_FRAMES%' onchange="calculateCaptureTime();"> 
        </div> 
        <div class="grid2" id='panAngleDiv'> 
          <h3>%STR_PAN_ANGLE%:</h3> 
          <input type='number' id='panAngle' placeholder='%STR_DEGREES%' onchange="calculateCaptureTime();"> 
        </div> 
        <div class="grid2" id='panDirectionDiv'> 
          <h3>%STR_PAN_DIRECTION%:</h3> 
          <select aria-label="panDirection" id='panDirection'> 
            <option value='0'>%STR_LEFT%</option> 
            <option value='1'>%STR_RIGHT%</option> 
          </select> 
        </div> 
        <div class="grid2" id='trackEndDiv'> 
          <h3>%STR_ENABLE_TRACKING%:</h3> 
          <select aria-label="enableTracking" id='enableTracking'> 
            <option value='0'>%STR_NO%</option> 
            <option value='1'>%STR_YES%</option> 
          </select> 
        </div> 
        <div class="grid2" id='ditherChoiceDiv'> 
          <h3>%STR_DITHER%:</h3> 
          <select aria-label="EnableDither" id='dither' onchange="chooseDither();"> 
            <option value='0'>%STR_NO%</option> 
            <option value='1'>%STR_YES%</option> 
          </select> 
        </div> 
        <div class="grid2" id='ditherFreqDiv'> 
          <h3>%STR_DITHER_FREQ%:</h3> 
          <input type="number" id="ditherFrequency" placeholder='%STR_NUM_EXPOSURES_HINT%' onchange="calculateCaptureTime();"> 
        </div> 
        <div class="grid2" id='ditherFocalLengthDiv'> 
          <h3>%STR_FOCAL_LENGTH%:</h3> 
          <input type="number" id="focalLength" placeholder='%STR_FOCAL_LENGTH_HINT%' onchange="calculateCaptureTime();"> 
        </div> 
        <div class="grid2" id='ditherPixelSizeDiv'> 
          <h3>%STR_PIXEL_SIZE%:</h3> 
          <input type="number" id="pixelSize" placeholder='%STR_PIXEL_SIZE_HINT%' step="0.01" onchange="calculateCaptureTime();"> 
        </div> 
        <div class="button-group"> 
          <button class="right-separator" type="button" id='capture-button' onclick="sendCaptureInformation('start');" disabled>%STR_START_CAPTURE%</button> 
          <button class="left-separator" type="button" onclick="sendRequest('/abort')">%STR_ABORT_CAPTURE%</button> 
        </div>
        <div > 
          <h3 id='captureTime'>%STR_CAPTURE_TIME%: 0 h, 0 m, 0 s</h3> 
        </div> 
      </div> 
      <div class="card"> 
        <h2>%STR_STATUS%</h2> 
        <h3 id='status'>%STR_STATUS_MSG%</h3> 
      </div> 
      <h4 id='firmware'>%STR_FIRMWARE_VERSION%: </h4>
    </div> 
  </body> 
  </html>
)=====";
/* clang-format off */
#endif
