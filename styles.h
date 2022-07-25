const char _styles[] PROGMEM = R"=====(
:root {
    /* global variables */
    --text : #fff;
    --bGradL: #343d46;
    --bGradR: #222831;
    --bg: var(--bGradL);
    --bOB: -webkit-linear-gradient(to right, var(--bGradL), var(--bGradR));
    --bDe: linear-gradient(to right, var(--bGradL), var(--bGradR));

    --appWidth: 75vw; 
    --mainInput: #525252;
    --mainInputColor: #eeeeee;
    --mainTextBoxW: 80vw;
    --mainBR: 9999px;
    --mainY : 70px;
    --hr: #777777;
    --hrWidth: 75vw;
    --topbarColor : #222831;
    --navhamlb : #c8c8c8;

    --navH : 92vh;
    --navW : 33vw;
    --navbarTextcolor: #c8c8c8;
    --navbarTextSize: 20px;
    --navbarHover: #525252;
    --navbarBg : #222831; 
    --navbarBorder: #5a5a5a;

    --sliderBg: #777777;
    --sliderThumb: #eeeeee;
    --sliderLabelColor: #eeeeee;
    --sliderLabelSize: 20px;
    --sliderWidth: 70%;
    --sliderLabelW: 30%;
    --sliderContW: 70%;

    --radioUnchecked : #525252;
    --radioChecked : #c8c8c8;
    --radioUncheckedText : #c8c8c8;
    --radioCheckedText : #313131;
    --radioUncheckedborder : #c8c8c8;
    --radioCheckedborder : #313131;

    --scrollWidth : 10px;
    --scrollTrackColor : #313131;
    --scrollThumbColor : #777777;
    --scrollThumbHoverColor : #c8c8c8;
    --scrollBorderColor: #777777;
    --scrollBorderRadius: 10px;

    --historyTextSize : 18px;
    --historyRowHover : #313131;
    --historyBorderRadius : 10px;
    --historyBorderColor: #777777;
    --historyPY : 10px;
    --historytextboxBg: #313131;
    --historyBtnHover: #313131;
    --historyBtnContWidth:30%;
    --historyTextboxWidth : 70%;

    --settingsTextSize : 18px;
    --settingsBorderRadius : 10px;
    --settingsBorderColor: #777777;
    --settingsPY : 10px;
    --settingsRowBg: #313131;
    --settingsBtnHover: #343d46;
    --settingsBtnContWidth:30%;
    --settingsTextboxWidth : 20%;

    --pixelY: 70px;
    --pixelBorderColor: #777777;
    --pixelBorderRadius: 10px;
    --pixelChecked : #FF9F29;
    --pixelUnchecked : #222831;
    --pixelBtnHover: #343d46;
    --pixelPY : 10px;
    --pixelControlsW : 80vw;
}


* {
    box-sizing: border-box;
    padding: 0;
    margin: 0;
    outline: none;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
}
::-webkit-scrollbar {
    width: var(--scrollWidth);
  }
  
  /* Track */
  ::-webkit-scrollbar-track {
    background: var(--scrollTrackColor); 
    border-radius: var(--scrollBorderRadius);
    border: 1px solid var(--scrollBorderColor);
  }
   
  /* Handle */
  ::-webkit-scrollbar-thumb {
    background: var(--scrollThumbColor);
    border-radius: var(--scrollBorderRadius);
  }
  
  /* Handle on hover */
  ::-webkit-scrollbar-thumb:hover {
    background: var(--scrollThumbHoverColor);
  }
.main-textbox {
    width: var(--mainTextBoxW);
    padding: 0px 25px;
    border: none;
    background: var(--mainInput);
    border-radius: var(--mainBR);
    color: var(--mainInputColor);
    font-size: 2em;
    height: var(--mainY);
    transition: all 0.3s ease-in-out;
    opacity: 0.8;
}
.pixel-controls {
    width: var(--pixelControlsW);
    height: var(--pixelY);
    border: 1px solid var(--pixelBorderColor);
    border-radius: var(--pixelBorderRadius);
}
.main-textbox:focus,
.main-textbox:hover {
    box-shadow: 0 0 20px var(--mainInputColor);
    opacity: 1;
}

.main-textbox:active {
    transform: scale(0.99);
}

body {
    background: var(--bg);
    background: var(--bOB);
    background: var(--bDe);
    font-family: Verdana, Geneva, Tahoma, sans-serif;
}

.title {
    display: flex;
    color: var(--title);
}
.text{
    color: var(--text);
}
/* width and height */
.mt-s {
    margin-top: 3vw;
}

.appWidth{
    width : var(--appWidth);
}


.h-25 {
    height: 25vh;
}

.h-33p {
    height: 33.33%;
}

.h-60 {
    height: 60vh;
}

.h-65 {
    height: 65vh;
}


.w-100p {
    width: 100%;
}

.container{
    width: 100vw;
    height: 100vh;
}

/* End of width and height */
/* Remove this */
.co {
    border: 2px solid red;
}

.co2 {
    border: 2px solid blue;
}

/* Divs */
.dfc {
    display: flex;
    align-items: center;
    flex-direction: column;
}

.dfcse {
    display: flex;
    align-items: center;
    flex-direction: column;
    justify-content: space-evenly;
}

.dfrfs {
    display: flex;
    flex-direction: row;
    justify-content: flex-start;
}
.dfrfsc{
    display: flex;
    flex-direction: row;
    align-items: center;
    justify-content: flex-start;
}
.dfcfs {
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: flex-start;
}

.dfrse {
    display: flex;
    flex-direction: row;
    justify-content: space-evenly;
    align-items: center;
}
.dfrsb{
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    align-items: center;
}
.dfrfe{
    display: flex;
    flex-direction: row;
    justify-content: flex-end;
    align-items: center;
}

/* End of Divs */
/* Sliders */

.slider {
    -webkit-appearance: none;
    width: var(--sliderWidth);
    height: 0.8em;
    border-radius: 5px;
    background: var(--sliderBg);
    outline: none;
    transition: filter 0.1s ease-in-out;
}

.slider::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;
    width: 2em;
    height: 2em;
    border-radius: 50%;
    background: var(--sliderThumb);
    cursor: pointer;
}

.slider::-moz-range-thumb {
    width: 2em;
    height: 2em;
    border-radius: 50%;
    background: var(--sliderThumb);
    cursor: pointer;
}

.slider-val {
    width: 50px;
    height: 30px;
    font-size: 20px;
    -moz-appearance: textfield;
    border-radius: 5px;
    border: none;
    text-align: center;
}

.slider-label{
    color: var(--sliderLabelColor);
    font-size: var(--sliderLabelSize);
    width: var(--sliderLabelW);
}
.slider-cont{
    width: var(--sliderContW);
}
input::-webkit-outer-spin-button,
input::-webkit-inner-spin-button {
    -webkit-appearance: none;
    margin: 0;
}

/* End of Sliders */
/* Radio buttons */
.radio-label{
    display: block;
    padding: 10px;
    background-color: var(--radioUnchecked);
    color : var(--radioUncheckedText);
    
    border-radius: 10px;
    border: 1px solid var(--radioUncheckedborder);
    cursor: pointer;
}

.radio-label:active{
    transform:scale(0.96);
}
.cpanel > input:checked ~ label{
    background-color: var(--radioChecked);
    color: var(--radioCheckedText);
    border: none;
}

/* Navbar Radio */
.radio-nav{
    width: 100%;
    height: 100%;
    cursor: pointer;
}
.radio-nav .checkmark{
    font-size: var(--navbarTextSize);
    color: var(--navbarTextcolor);
    width : 100%;
    height : 100%;
}
.radio-nav:active input ~ .checkmark {
    transform:scale(0.96);
}
.radio-nav input:checked ~ .checkmark{
    background-color: var(--navbarHover);
    border: none;
}
/* End of Radio buttons */
/* Basic Tags */
.dn {
    display: none;
}

/* End of Basic Tags*/
/*custom*/
.hr {
    width: var(--hrWidth);
    border: 1px solid var(--hr);
}
.topbar{
    display: flex;
    height: 8vh;
    width: 100%;
    flex-direction: row;
    justify-content: flex-start;
    align-items: center;
    background-color: var(--topbarColor);
}
#navham{
    display: none;
}
.navhamlb{
    display: flex;
    height: 4.8vh;
    width: calc((4.8vh - (4.8vh*0.1))*1.414);
    flex-direction: column;
    justify-content: space-between;
    cursor: pointer;
    margin-left: 10px;
}
.navhamlb span{
    display: block;
  height: 15%;
  width: 100%;
  border-radius: 10px;
  background: var(--navhamlb);
}
.nline1 {
    transform-origin: 0% 0%;
    transition: transform 0.4s ease-in-out;
  }
  
  .nline2 {
    transition: transform 0.2s ease-in-out;
  }
  
  .nline3 {
    transform-origin: 0% 100%;
    transition: transform 0.4s ease-in-out;
  }
  

#navham:checked ~ .container .topbar .navhamlb .nline1{
    transform: rotate(45deg);
}
#navham:checked ~ .container .topbar .navhamlb .nline2{
    transform: scaleY(0);
}
#navham:checked ~ .container .topbar .navhamlb .nline3{
    transform: rotate(-45deg);
}
#navham:checked ~ .container .navbar{
   transform: translateX(0);
}
.navbar {
    height: var(--navH);
    width: var(--navW);
    position:absolute;
    left: 0;
    top: 8vh;
    transform: translateX(-150%);
    display: flex;
    align-items: center;
    flex-direction: column;
    justify-content: flex-start;
    z-index: 5;
    background-color: var(--navbarBg);                                                                         
    border: 1px solid var(--navbarBorder);
    overflow-y: auto;
    transition: transform 0.4s ease-in-out;
}



#ID_HISTORY_TEXT, #ID_SETTINGS_TEXT{
    overflow-y: auto;
}

.history-row{
    width: 100%;
    margin-top: var(--historyPY);
    margin-bottom: var(--historyPY);
}
.history-text-row{
    padding-top: var(--historyPY);
    padding-bottom: var(--historyPY);
    font-size: var(--historyTextSize);
    width: var(--historyTextboxWidth);
    border: 1px solid var(--historyBorderColor);
    border-radius: var(--historyBorderRadius);
    overflow: hidden;
}

.history-text-row:hover{
    cursor: pointer;
    background-color: var(--historyRowHover);
}
.history-text-row:active{
    transform: scale(0.96);
}


.history-btn{
    padding: var(--historyPY);
    font-size: var(--historyTextSize);
    border: 1px solid var(--historyBorderColor);
    border-radius: var(--historyBorderRadius);
    
}
.history-btn-cont{
    width: var(--historyBtnContWidth);
}
.history-btn:hover{
    cursor: pointer;
    background-color: var(--historyBtnHover);
    
}
.history-btn:active{
    transform: scale(0.96);
}
.history-textbox-cont{
    width: var(--historyTextboxWidth);
}
.history-textbox{
    padding: 10px;
    font-size: var(--historyTextSize);
    background-color: var(--historytextboxBg);
    border: 1px solid var(--historyBorderColor);
    border-radius: var(--historyBorderRadius);
    width: 100%;
}


.settings-row{
    width: 95%;
    margin-top: var(--settingsPY);
    margin-bottom: var(--settingsPY);
    background-color: var(--settingsRowBg);
    font-size: var(--settingsTextSize);
    border: 1px solid var(--settingsBorderColor);
    border-radius: var(--settingsBorderRadius);
}
.settings-text{
    margin: var(--settingsPY);
}
.settings-btn{
    padding: var(--settingsPY);
    margin: var(--settingsPY);
    
    border: 1px solid var(--settingsBorderColor);
    border-radius: var(--settingsBorderRadius);
    text-align: center;
}
.settings-btn-c{
    width: var(--settingsBtnContWidth);
}
.settings-btn:hover{
    cursor: pointer;
    background-color: var(--settingsBtnHover);
    
}
.settings-btn:active{
    transform: scale(0.96);
}

.settings-textbox-cont{
    width: var(--settingsTextboxWidth);
    margin: var(--settingsPY);
}
.settings-textbox{
    padding: 10px;
    font-size: var(--settingsTextSize);
    background-color: var(--settingstextboxBg);
    border: 1px solid var(--settingsBorderColor);
    border-radius: var(--settingsBorderRadius);
    width: 100%;
}

#ID_PIXEL_BOARD{
    border: 1px solid var(--pixelBorderColor);
    border-radius: var(--pixelBorderRadius);
    width: min(calc(65vh*2),75vw);
    height: min(calc(75vw * 0.5), 65vh);
}
.pixelart-row{
    width: 100%;
    height: 12.5%
}
.pixelart-col{
    width: 6.25%;
    height: 100%;
}
.pixel-dot{
    width: 75%;
    height: 75%;
    background-color: var(--pixelUnchecked);
    border: 1px solid var(--pixelBorderColor);
    border-radius: 50%;
}
.pixel-dot-ch{
    background-color: var(--pixelChecked);
    border: 1px solid var(--pixelBorderColor);
    border-radius: 50%;
}
.pixel-c-btn{
    padding: var(--pixelPY);
    border: 1px solid var(--pixelBorderColor);
    border-radius: var(--pixelBorderRadius);
    cursor: pointer;
}

#ID_PIXEL_ERASE:checked ~ label , .pixel-c-btn:hover{
    background-color: var(--pixelBtnHover);
}
.pixel-c-btn:active{
    transform: scale(0.96);
}
/* End of custom */
@media only screen and (max-width: 600px) {
:root{
    --appWidth: 95vw;

    --navW: 50vw;
    
    --sliderContW: 80%;
    --sliderWidth: 60%;
    --sliderLabelW: 40%;

    --settingsBtnContWidth:50%;
    --settingsTextboxWidth: 80%;

    --pixelControlsW : 90vw;
}
.collapse{
    flex-direction: column;
}
}

)=====";