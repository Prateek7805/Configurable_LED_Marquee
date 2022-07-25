const HOST_URL = window.location.origin;
var Socket;
const uriList = {
    speed: "sb",
    bright: "sb",
    settings: "setting",
    creds: "creds",
    pixels: "pixels",
    pixelUpdate: "pixelupdate"
}

var historyPos = [];

var historyData = [];
var runtime = {
    status: { row: 8, col: 16 },
    rename: { status: false, id: "", text: "" },
    pixel: { posX: -1, posY: -1, state: [], rmousedn: false, lmousedn: false, click: false, erase: false, leftS: [], rightS: [], ln: 16 },
}
// Builder functions
function dq(i) {
    return document.querySelectorAll(i);
}
function d(i) {
    return document.getElementById(i);
}
function dt(i) {
    return document.getElementsByTagName(i);
}
function dae(o, e, f) {
    o.addEventListener(e, f);
}
function dar(o, e, f) {
    o.removeEventListener(e, f);
}
function dc(c) {
    return document.getElementsByClassName(c);
}
function dca(c) {
    return document.getElementsByClassName(c);
}
function dnone(o) {
    o.style.display = 'none';
}
function dflex(o) {
    o.style.display = 'flex';
}
function dchange(o, cur, st) {

    o.classList.replace(cur, st);
}
const er = (m) => {
    console.log(m);
}
const _send = (head, data) => {
    er(head + data);
    Socket.send(head + data);
}
const disEn = (o, status) => {
    o.disabled = status;
}
const connect = () => {
    Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
    var connection = d('connection');

    Socket.onmessage = (event) => {
        er(event.data);
    }
    Socket.onopen = () => {
        er("Socket connected.");
    }
    Socket.onclose = (e) => {
        er('Socket is closed. Reattempting..');
        setTimeout(function () {
            connect();
        }, 250);
    };

    Socket.onerror = (err) => {


        Socket.close();
    };
}
const fPOST = (uri, arg, fun) => {
    bodyArg = ""
    arg.forEach(i => {
        bodyArg += `${i.param}=${i.value}&`;
    });
    bodyArg = bodyArg.substring(0, bodyArg.length - 1);
    fetch(`${HOST_URL}/${uri}`, {
        method: "POST",
        headers: {
            'Content-type': 'application/x-www-form-urlencoded'
        },
        body: bodyArg
    }).then(response => response.text()).then(fun);
}
const fGET = (uri, arg, fun) => {
    bodyArg = ""

    arg.forEach(i => {
        bodyArg += `${i.param}=${i.value}&`;
    });
    bodyArg = "?" + bodyArg.substring(0, bodyArg.length - 1);

    fetch(`${HOST_URL}/${uri}${bodyArg}`).then(response => response.text()).then(fun);
}

const clearLedFn = () => {
    Array.prototype.forEach.call(dq(`input[type=radio][name=LedDisplay]`), (item) => {
        item.checked = false;
    });
}
//navbar and Pixel ART
const displayScreen = (id) => {
    const Screens = {
        controls: {
            id: ['CONTROLS', 'MAIN_TEXTBOX'],
            cl: ['dfrfs', 'dfrfs']
        },
        history: {
            id: ['HISTORY', 'MAIN_TEXTBOX'],
            cl: ['dfrfs', 'dfrfs']
        },
        settings: {
            id: ['SETTINGS', 'MAIN_TEXTBOX'],
            cl: ['dfrfs', 'dfrfs']
        },
        pixelart: {
            id: ['PIXELART', 'PIXEL_CONTROLS'],
            cl: ['dfcfs', 'dfrse']
        }
    };
    for (const item in Screens) {
        Screens[item].id.forEach((i, k) => dchange(d(`ID_${i}`), Screens[item].cl[k], 'dn'));
    }
    for (const item in Screens) {
        if (Screens[item].id[0] == id) {
            Screens[item].id.forEach((i, k) => { dchange(d(`ID_${i}`), 'dn', Screens[item].cl[k]) });
        }
    }

}
const drawPixelBoard = (row, col) => {
    const PIXEL_CONT = d('ID_PIXEL_BOARD');
    const width = PIXEL_CONT.offsetWidth;
    const height = PIXEL_CONT.offsetHeight;
    var dim = (width < height) ? width : height;
    var div = (width < height) ? col : row;
    dim = (dim / div) * 0.6;
    PIXEL_CONT.innerHTML = "";
    if (runtime.pixel.state.length === 0) {
        for (var i = 0; i < col; i++) { runtime.pixel.state[i] = 0 }
    }
    for (var i = 0; i < row; i++) {
        const PIXEL_ROW = document.createElement("div");
        PIXEL_ROW.setAttribute("class", "dfrfs  pixelart-row");
        PIXEL_ROW.setAttribute("ID", `pir${i}`);
        PIXEL_CONT.appendChild(PIXEL_ROW);
        for (var j = 0; j < col; j++) {
            const PIXEL_COL = document.createElement("div");
            PIXEL_COL.setAttribute("class", "dfrse pixelart-col");
            const n = i * col + j;
            PIXEL_COL.setAttribute("ID", `pic${n}`);
            PIXEL_ROW.appendChild(PIXEL_COL);
            const LED = document.createElement("div");
            if ((runtime.pixel.state[j] >> i) & 0x01) {
                LED.setAttribute("class", "dfrfs pixel-dot-ch");
            } else {
                LED.setAttribute("class", "dfrfs pixel-dot");
            }
            LED.setAttribute("ID", `dot${n}`);
            PIXEL_COL.appendChild(LED);
            d(`dot${n}`).style.width = dim + 'px';
            d(`dot${n}`).style.height = dim + 'px';
        }
    }
}

const pixelDisplayInit = () => {
    drawPixelBoard(8, 16);
}
Array.prototype.forEach.call(dq(`input[type=radio][name=navbar]`), (item) => {
    dae(item, 'change', (e) => {
        var value = e.target.value;
        displayScreen(value.substring(value.lastIndexOf('_') + 1));
        d("navham").checked = false;
    });
    if (item.checked) {
        displayScreen(item.value.substring(item.value.lastIndexOf('_') + 1));
        if (item.id == 'PIXEL_ART_RADIO') {
            pixelDisplayInit();
            clearLedFn();
        }

    }
});

const colorPixelDisplay = (e, row, col) => {
    if (!(runtime.pixel.lmousedn || runtime.pixel.rmousedn)) {
        return;
    }

    const PIXEL_CONT = d('ID_PIXEL_BOARD');

    const width = PIXEL_CONT.offsetWidth;
    const height = PIXEL_CONT.offsetHeight;
    const WIDTH = window.innerWidth;
    var wDim = width / col;
    var hDim = height / row;
    var diffW = (WIDTH - width) / 2;
    const HTop = d("ID_TOP_BAR").offsetHeight;
    const H_INPUT = d("ID_CENTER_WRAPPER").offsetHeight;
    var diffH = HTop+H_INPUT;
    var posX = parseInt((e.clientX - diffW) / wDim);
    var posY = parseInt((e.clientY - diffH) / hDim);
    const n = col * posY + posX;
    const LED = d(`dot${n}`);
    const lDim = LED.getBoundingClientRect();
    if ((e.clientX >= lDim.x && e.clientX <= (lDim.x + lDim.width)) && (e.clientY >= lDim.y && e.clientY <= (lDim.y + lDim.height))) {
        if (runtime.pixel.posX == posX && runtime.pixel.posY == posY) {
            return;
        }

        runtime.pixel.posX = posX;
        runtime.pixel.posY = posY;

        if (runtime.pixel.lmousedn) {
            dchange(LED, "pixel-dot", "pixel-dot-ch");
            runtime.pixel.state[posX] |= (1 << posY);
        } else if (runtime.pixel.rmousedn) {
            dchange(LED, "pixel-dot-ch", "pixel-dot");
            runtime.pixel.state[posX] &= ~(1 << posY);
        }
        _send("p", `${posX},${runtime.pixel.state[posX]}`);
    } else {
        runtime.pixel.posX = -1;
        runtime.pixel.posY = -1;
    }
}

const colorPixels = (e) => colorPixelDisplay(e, 8, 16);

//End of Navbar and Pixel ART
const validateNumber = (val) => {
    if (!val.match(/^([0-9]?[0-9]?\d)$/)) {
        return false;
    }
    if (parseInt(val) > 100) {
        return false;
    }
    return true;
}
//Sliders
dae(d('ID_SPEED'), 'input', (e) => {
    d('ID_SPEED_VAL').value = e.target.value;
});
dae(d('ID_SPEED_VAL'), 'input', (e) => {
    var p = e.target.value;
    if (validateNumber(p)) {
        d('ID_SPEED').value = p;
        return;
    }
    d('ID_SPEED').value = 0;
    d('ID_SPEED_VAL').value = '';
});
dae(d('ID_BRIGHT'), 'input', (e) => {
    d('ID_BRIGHT_VAL').value = e.target.value;
});
dae(d('ID_BRIGHT_VAL'), 'input', (e) => {
    var p = e.target.value;
    if (validateNumber(p)) {
        d('ID_BRIGHT').value = p;
        return;
    }
    d('ID_BRIGHT').value = 0;
    d('ID_BRIGHT_VAL').value = '';
});
//End of Sliders
//Text Operations
const limitTextSize = (text) => {
    return text.length > 20 ? (text.substring(0, 20) + "...") : text;
}
const pushHistory = (txt, index) => {
    const HISTORY = d('ID_HISTORY_TEXT');
    const type = index[0];
    const text = (type == 't') ? txt: `PixelSave${index.substring(1)}`;
    HISTORY.innerHTML += `<div class="dfrse history-row" id="hc${index}">
                                <div class="dfrse text history-text-row" id="hh${index}" onclick="sendText(this)">${limitTextSize(text)}</div>
                                <div class="dfrse history-btn-cont">
                                    <div class="dfrse text history-btn ${(type == 'p') ? 'dn' : ''}" id="hr${index}" onclick="rnTextbox(this)">Rename</div>
                                    <div class="dfrse text history-btn" id="hd${index}" onclick="deleteText(this)">Delete</div>
                                </div>
                          </div>`;
}
const getHistoryText = (pos) => {

    for (var i = 0; i < historyData.length; i++) {
        if (historyData[i].p == pos) {
            return historyData[i].t;
        }
    }
    return "";
}
const updateHistoryText = (pos, text) => {

    for (var i = 0; i < historyData.length; i++) {
        if (historyData[i].p == pos) {
            historyData[i].t = text;
            return;
        }
    }
}
const rnTextbox = (self) => {
    const id = self.id.substring(2);
    const historyBox = d(`hc${id}`);
    const historyText = d(`hh${id}`);
    const rnBtn = d(`hr${id}`);

    const DIV = document.createElement("div");
    DIV.setAttribute("id", `hh${id}`);
    if (rnBtn.innerHTML === "Rename") {
        if (runtime.rename.status) {

            rnTextbox(runtime.rename);
        }
        const text = getHistoryText(id);
        runtime.rename.status = true;
        runtime.rename.id = self.id;
        runtime.rename.text = text;
        DIV.setAttribute("class", "dfrse history-textbox-cont");
        const textbox = document.createElement("input");
        textbox.setAttribute("type", "text");
        textbox.setAttribute("class", "history-textbox text");
        textbox.setAttribute("value", text);
        textbox.setAttribute("id", `ht${id}`);
        DIV.appendChild(textbox);
        historyBox.replaceChild(DIV, historyText);
        rnBtn.innerHTML = "Cancel";
        textbox.focus();
        textbox.setSelectionRange(text.length, text.length, 'forward');
        runtime.rename.status = true;
        dae(textbox, 'keypress', sendRnText);
        return;
    }
    DIV.setAttribute("class", "dfrse text history-text-row");
    DIV.setAttribute("onclick", "sendText(this)");
    DIV.innerHTML = limitTextSize(runtime.rename.text);
    dar(d(`ht${id}`), 'keypress', sendRnText);
    runtime.rename.status = false;
    historyBox.replaceChild(DIV, historyText);
    rnBtn.innerHTML = "Rename";

}
const sendRnText = (e) => {
    if (e.key == 'Enter' && e.target.value != "") {
        if (e.target.value === runtime.rename.text) {
            rnTextbox(e.target);
            return;
        }
        const pos = e.target.id.substring(2);
        const text = e.target.value.trim();
        fPOST("rename", [{ param: "ind", value: pos }, { param: "data", value: text }], () => {
            runtime.rename.text = text;
            updateHistoryText(pos, text);
            rnTextbox(e.target);
        })
    }

}
const deleteText = (self) => {
    fGET("delete", [{ param: "ind", value: self.id.substring(2) }], () => {
        const HISTORY = d('ID_HISTORY_TEXT');
        HISTORY.removeChild(d(`hc${self.id.substring(2)}`));
    });
}
const sendText = (self) => {
    fGET("disp", [{ param: "ind", value: self.id.substring(2) }], () => { });
}
// End of Text Operations
//Settings
const updateWMODE = (MODE) => {
    const IDS = ["ID_S_WMODE_STA", "ID_S_WMODE_AP"];
    const style = getComputedStyle(document.body);
    const colorA = style.getPropertyValue('--settingsRowBg');
    const colorB = style.getPropertyValue('--settingsBtnHover');
    IDS.forEach(i => { d(i).style.backgroundColor = colorA });
    d(`ID_S_WMODE_${MODE}`).style.backgroundColor = colorB;
}
const sendSettings = (type, val, fun) => {
    fGET(uriList.settings, [{ param: "type", value: type }, { param: "val", value: val }], fun);
}
const clearCredsText = (PRE) => {
    const SSID_ID = PRE + "_SSID";
    const PASS_ID = PRE + "_PASS";
    d(SSID_ID).value = "";
    d(PASS_ID).value = "";

}
const validateCred = (SSID, PASS) => {
    if (!SSID.match(/^[^!#;+\]\/"\t][^+\]"\t]{0,31}$/)) {
        return false;
    } if ((PASS == "") || !PASS.match(/^[ -~]{8,63}$/)) {
        return false;
    }
    return true;
}
const sendCreds = (id) => {
    const PRE = id.substring(0, id.lastIndexOf('_'));
    const SSID_ID = PRE + "_SSID";
    const PASS_ID = PRE + "_PASS";
    const SSID = d(SSID_ID).value;
    const PASS = d(PASS_ID).value;
    if (!validateCred(SSID, PASS)) {
        clearCredsText(PRE);
        return;
    }
    const type = PRE.substring(PRE.lastIndexOf('_') + 1);
    fPOST(uriList.creds, [{ param: "type", value: type }, { param: "ssid", value: SSID }, { param: "pass", value: PASS }], clearCredsText(PRE));
}

const settingsList = [
    {
        id: "ID_S_WMODE_STA",
        e: "click",
        fun: () => { sendSettings("WMODE", 0, () => { updateWMODE("STA") }) }
    },
    {
        id: "ID_S_WMODE_AP",
        e: "click",
        fun: () => { sendSettings("WMODE", 1, () => { updateWMODE("AP") }) }
    },
    {
        id: "ID_S_CREDS_STA_SUBMIT",
        e: "click",
        fun: () => { sendCreds("ID_S_CREDS_STA_SUBMIT") }
    },
    {
        id: "ID_S_CREDS_AP_SUBMIT",
        e: "click",
        fun: () => { sendCreds("ID_S_CREDS_AP_SUBMIT") }
    },
    {
        id: "ID_S_RESTART",
        e: "click",
        fun: () => { sendSettings("RESET", 0, () => { }) }
    },
    {
        id: "ID_S_RELOAD",
        e: "click",
        fun: () => { sendSettings("RESET", 1, () => { }) }
    },
    {
        id: "ID_S_FRESET",
        e: "click",
        fun: () => { sendSettings("RESET", 2, () => { }) }
    },
    {
        id: "PIXEL_ART_RADIO",
        e: "change",
        fun: () => {
            clearLedFn();
            pixelDisplayInit();
            sendSettings("anim", "pixel", () => { });
        }
    },
    {
        id: "ID_SPEED",
        e: "change",
        fun: (e) => { sendSettings("speed", e.target.value, () => { }) }
    },
    {
        id: "ID_SPEED_VAL",
        e: "change",
        fun: (e) => { sendSettings("speed", e.target.value, () => { }) }
    },
    {
        id: "ID_BRIGHT",
        e: "change",
        fun: (e) => { sendSettings("bright", e.target.value, () => { }) }
    },
    {
        id: "ID_BRIGHT_VAL",
        e: "change",
        fun: (e) => { sendSettings("bright", e.target.value, () => { }) }
    }
];
//End of Settings
//Pixel Event List
const pixToHex = (val) => {
    return (val <= 0x0F) ? `0${val.toString(16)}` : val.toString(16);
}
const clearDisplay = () => {
    runtime.pixel.state = [];
    runtime.pixel.leftS = [];
    runtime.pixel.rightS = [];
    pixelDisplayInit();
}
const pixelClear = () => {
    clearDisplay();
    _send('c', '');
}
const pixErase = () => {
    runtime.pixel.erase = !runtime.pixel.erase;
}
const pixInvert = () => {
    runtime.pixel.state = runtime.pixel.state.map(x => (0xff - x));
    pixelDisplayInit();
    _send('i', '');
}
const pixLeft = () => {

    const ln = runtime.pixel.ln;
    const lf = runtime.pixel.state[0];
    runtime.pixel.leftS.push(lf || 0);
    for (var i = 0; i < ln - 1; i++) {
        runtime.pixel.state[i] = runtime.pixel.state[i + 1];
    }
    const val = runtime.pixel.rightS.pop() || 0;
    runtime.pixel.state[ln - 1] = val;
    pixelDisplayInit();
    _send('l', '');
}
const pixRight = () => {
    const ln = runtime.pixel.ln;
    const rn = runtime.pixel.state[ln - 1];
    runtime.pixel.rightS.push(rn || 0);
    for (var i = ln - 1; i > 0; i--) {
        runtime.pixel.state[i] = runtime.pixel.state[i - 1];
    }
    const val = runtime.pixel.leftS.pop() || 0;
    runtime.pixel.state[0] = val;
    pixelDisplayInit();
    _send('r', '');
}
const pixelSave = () => {
    clearDisplay();
    _send('s', ''); 
    appendHistory('p', '');
}
const PIXEL_EVENTS = [
    {
        id: "ID_PIXEL_CLEAR",
        e: "click",
        fun: pixelClear
    },
    {
        id: "ID_PIXEL_ERASE",
        e: "click",
        fun: pixErase
    }, {
        id: "ID_PIXEL_INVERT",
        e: "click",
        fun: pixInvert
    }, {
        id: "ID_PIXEL_LEFT",
        e: "click",
        fun: pixLeft
    }, {
        id: "ID_PIXEL_RIGHT",
        e: "click",
        fun: pixRight
    }, {
        id: "ID_PIXEL_SAVE",
        e: "click",
        fun: pixelSave
    }
];

const maxHPos = () => {
    if (historyPos.length == 0) {
        return 0;
    }
    var max = parseInt(historyPos[0].substring(1));
    historyPos.forEach(i => {
        const v = parseInt(i.substring(1));
        if (v > max) {
            max = v;
        }
    });
    return max;
}

const appendHistory = (type, val) => {
    let maxPos = `${type}${maxHPos() + 1}`;
    historyData.push({ p: maxPos, t: val });
    pushHistory(val, maxPos);
    historyPos.push(maxPos);
}

const pixelInput = (e, val) => {
    e = e || window.event;
    runtime.pixel.posX = -1;
    runtime.pixel.posY = -1;
    var wh = e.which;

    wh = (runtime.pixel.erase) ? ((wh == 1) ? 3 : 1) : wh;
    switch (wh) {
        case 1:
            runtime.pixel.lmousedn = val;
            colorPixels(e);
            break;
        case 3:
            runtime.pixel.rmousedn = val;
            colorPixels(e);
            break;
    }
}
//End of Pixel Event List
dae(document, 'DOMContentLoaded', () => {


    fetch(`${HOST_URL}/status`).then(r => r.text()).then(data => {
        data = JSON.parse(data);
        d('ID_SPEED').value = data.S;
        d('ID_SPEED_VAL').value = data.S;
        d('ID_BRIGHT').value = data.B;
        d('ID_BRIGHT_VAL').value = data.B;
        runtime.status.row = data.R;
        runtime.status.col = data.C;
        er(data);
    });
    fetch(`${HOST_URL}/history`).then(r => r.text()).then(data => {
        if (data == "[]") {
            return;
        }
        er(data);
        data = data.substring(0, data.length - 2) + ']';
        data = JSON.parse(data);

        data.forEach(item => {
            historyData.push(item);
            pushHistory(item.t, item.p);
            historyPos.push(item.p);
        });
    });
    fetch(`${HOST_URL}/${uriList.pixels}`).then(r => r.text()).then(data => {
        if (data == "[]") {
            return;
        }
        er(data);

        data = JSON.parse(data);
        runtime.pixel.leftS = data[0];
        runtime.pixel.state = data[1];
        runtime.pixel.rightS = data[2];
        pixelDisplayInit();
    });
    connect();
    dae(window, 'resize', () => {
        if (!d('PIXEL_ART_RADIO').checked) {
            return;
        }
        pixelDisplayInit();
    });
    dae(d('ID_PIXEL_BOARD'), 'mousedown', (e) => {
        pixelInput(e, true);
    });
    dae(window, 'mouseup', (e) => {
        pixelInput(e, false);
    });
    dae(d('ID_PIXEL_BOARD'), 'touchstart', (e) => {
        const inp = {which : 1, clientX: e.touches[0].clientX, clientY : e.touches[0].clientY};
        pixelInput(inp, true);
    });
    dae(d('ID_PIXEL_BOARD'), 'touchend', (e) => {
        const inp = {which : 1, clientX: e.changedTouches[0].clientX, clientY : e.changedTouches[0].clientY};
        pixelInput(inp, false);
    });
    dae(d('ID_PIXEL_BOARD'), 'touchmove', (e) => {
        const inp = {clientX: e.touches[0].clientX, clientY : e.touches[0].clientY};
        colorPixels(inp);
    });
    dae(d('ID_PIXEL_BOARD'), 'mousemove', colorPixels);


    dae(d('ID_MAIN_TEXTBOX'), 'keypress', (e) => {
        if (e.key == 'Enter' && e.target.value != "") {
            fPOST("text", [{ param: "data", value: e.target.value.trim() }], () => {
                appendHistory('t', e.target.value);
                d("ID_MAIN_TEXTBOX").value = "";
            });
        }
    });
    PIXEL_EVENTS.forEach(item => {
        dae(d(item.id), item.e, item.fun);
    });
    settingsList.forEach(item => {
        dae(d(item.id), item.e, item.fun);
    });
    Array.prototype.forEach.call(dq(`input[type=radio][name=LedDisplay]`), (item) => {
        dae(item, 'change', (e) => {
            var value = e.target.value;
            sendSettings("anim", value, () => { });
        });
    });
});