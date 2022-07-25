const char _index[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="styles.css">
    <title>ConfigurableLEDMarquee</title>

</head>

<body oncontextmenu="return false">
    <input type="checkbox" id="navham">
    <div class="container dfc">
        <div class="topbar" id="ID_TOP_BAR">
            
            <label for="navham" class="navhamlb">
                <span class="nline1"></span>
                <span class="nline2"></span>
                <span class="nline3"></span>
            </label>   
            
        </div>
        <div class="navbar">
            <label class="radio-nav">
                <input class="dn CL_RADIO_NAV" value="NAV_CONTROLS" type="radio" name="navbar">
                <span class="dfcse checkmark">Controls</span>
            </label>
            <label class="radio-nav">
                <input class="dn CL_RADIO_NAV" value="NAV_HISTORY" type="radio" name="navbar">
                <span class="dfcse checkmark">History</span>
            </label>
            <label class="radio-nav">
                <input class="dn CL_RADIO_NAV" value="NAV_SETTINGS" type="radio" name="navbar">
                <span class="dfcse checkmark">Settings</span>
            </label>
            <label class="radio-nav">
                <input class="dn CL_RADIO_NAV" value="NAV_PIXELART" id="PIXEL_ART_RADIO" type="radio" checked="checked" name="navbar">
                <span class="dfcse checkmark">Pixel Art</span>
            </label>
        </div>

        <div class="dfcse h-25" id="ID_CENTER_WRAPPER">
            <input id="ID_MAIN_TEXTBOX" class="dn main-textbox w-80" placeholder="" />
            <div id="ID_PIXEL_CONTROLS" class="dn pixel-controls">
                
                <div class="dfrse pixel-c-btn text" id="ID_PIXEL_CLEAR">Clear</div>
                <div>
                    <input type="checkbox" class="dn" id="ID_PIXEL_ERASE"/>
                    <label for="ID_PIXEL_ERASE" class="dfrse pixel-c-btn text">Erase</label>
                </div>
                
                <div class="dfrse pixel-c-btn text" id="ID_PIXEL_LEFT">&lt;&lt;</div>
                <div class="dfrse pixel-c-btn text" id="ID_PIXEL_RIGHT">&gt;&gt;</div>
                <div class="dfrse pixel-c-btn text" id="ID_PIXEL_SAVE">save</div>
                <div class="dfrse pixel-c-btn text" id="ID_PIXEL_INVERT">Invert</div>
            </div>
            <hr class="w-75 hr">
        </div>
        <!--Main window-->
        <div class="dfrfs appWidth h-65" id="ID_CONTROLS">
            <div class="dfcfs w-100p h-60">
                <div class="dfrse h-33p w-100p">
                    <span class="cpanel">
                    <input class="dn" type="radio" id="scroll" checked="checked" value="scroll" name="LedDisplay">
                    <label for="scroll" class="dfrse radio-label">Scroll</label>
                    </span>
                    <span class="cpanel">
                    <input class="dn" type="radio" id="ledone" value="ledone" name="LedDisplay">
                    <label for="ledone" class="dfrse radio-label">One/time</label>
                    </span>
                    <span class="cpanel">
                    <input class="dn" type="radio" id="ledoff" value="ledoff" name="LedDisplay">
                    <label for="ledoff" class="dfrse radio-label">Off</label>
                    </span>
                </div>
                <div class="dfrse collapse h-33p w-100p">
                    <p class="slider-label">Speed</p>
                    <div class="dfrsb  slider-cont">  
                        <input type="range" id="ID_SPEED" class="slider" min='0' max='100' value="0">
                        <input type="number" id="ID_SPEED_VAL" class="slider-val" min="0" max="100" value="0">
                    </div>
                    
                </div>

                <div class="dfrse collapse h-33p w-100p">
                    <p class="slider-label">Brightness</p>
                    <div class="dfrsb slider-cont">
                        
                        <input type="range" id="ID_BRIGHT" class="slider" min='0' max='100' value="0">
                        <input type="number" id="ID_BRIGHT_VAL" class="slider-val" min="0" max="100" value="0">
                    </div>
                    
                </div>
                
            </div>

        </div>
        <!--HISTORY-->
        <div class="dfrfs appWidth h-65" id="ID_HISTORY">
            <div class="dfrfs  w-100p h-60">
                <div class="dfcfs w-100p" id="ID_HISTORY_TEXT">
                    
                </div>
            </div>
        </div>
        <!--SETTINGS-->
        <div class="dfrfs appWidth h-65" id="ID_SETTINGS">
            <div class="dfrfs w-100p h-60">
                <div class="dfcfs w-100p" id="ID_SETTINGS_TEXT">
                    <div class="dfrfsc settings-row">
                        <div class="dfrfs settings-text text">WiFi Mode :</div>
                        <div class="dfrse text settings-btn-c">
                            <div class="dfrse settings-btn" id="ID_S_WMODE_STA">STA</div>
                            <div class="dfrse settings-btn" id="ID_S_WMODE_AP">AP</div>
                        </div>
                    </div>
                    <div class="dfrfsc settings-row collapse">
                        <div class="dfrfs settings-text text">STA Credentials:</div>
                        <div class="dfrfs settings-textbox-cont"><input type="text" class="settings-textbox text"
                                placeholder="Enter SSID" id="ID_S_CREDS_STA_SSID"></div>
                        <div class="dfrfs settings-textbox-cont"><input type="text" class="settings-textbox text"
                                placeholder="Enter Password" id="ID_S_CREDS_STA_PASS"></div>
                            <div class="dfrse text settings-btn" id="ID_S_CREDS_STA_SUBMIT">Submit</div>
                    </div>
                    <div class="dfrfsc settings-row collapse">
                        <div class="dfrfs settings-text text">AP Credentials:</div>
                        <div class="dfrfs settings-textbox-cont"><input type="text" class="settings-textbox text"
                                placeholder="Enter SSID" id="ID_S_CREDS_AP_SSID"></div>
                        <div class="dfrfs settings-textbox-cont"><input type="text" class="settings-textbox text"
                                placeholder="Enter Password" id="ID_S_CREDS_AP_PASS"></div>               
                            <div class="dfrse text settings-btn" id="ID_S_CREDS_AP_SUBMIT">Submit</div>            
                    </div>
                    <div class="dfrse settings-row collapse">
                        <div class="dfrse text settings-btn" id="ID_S_RESTART">Restart</div>
                        <div class="dfrse text settings-btn" id="ID_S_RELOAD">Reload</div>
                       <div class="dfrse text settings-btn" id="ID_S_FRESET">Factory Reset</div>
                    </div>
                </div>

            </div>
        </div>
        <div class="dfcfs appWidth h-65" id="ID_PIXELART">
            <div class="dfcfs" id="ID_PIXEL_BOARD">
                
            </div>
        </div>
    </div>


    
    </div>
    <script src="script.js"></script>
</body>

</html>
)=====";