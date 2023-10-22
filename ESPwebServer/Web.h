String pagina=R"====(<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>ESPintar</title>
        <style>
            body{display:grid;margin:0;padding:0;font-family:system-ui,-apple-system,BlinkMacSystemFont,Segoe UI,Roboto,Oxygen,Ubuntu,Cantarell,Open Sans,Helvetica Neue,sans-serif;background-color:#0c0c0c;color:#fff}header{display:grid;grid-template-columns:1fr 3fr;background:#0c0c0c;color:#fff;height:10svh;font-weight:800}.title{display:flex;justify-content:center;grid-column:2}#content{display:grid;justify-content:center;align-items:center;height:90svh;grid-template-columns:1fr 3fr}@media only screen and (max-width: 768px){header{grid-template-columns:1fr}.title{grid-column:1}#content{grid-template-columns:1fr}#grid-container{grid-row:1}}:root{--grid-size: 16}#grid-container{display:flex;height:100%;align-items:center;justify-content:center;background-color:#222;border-radius:5px}#grid{justify-self:center;display:grid;grid-template-columns:repeat(var(--grid-size),1fr);grid-template-rows:repeat(var(--grid-size),1fr);width:clamp(320px,50vw,550px);height:clamp(320px,50vw,550px);background-color:#0c0c0c;padding:5px;gap:5px}.grid-cell{border:1px solid #ccc;background-color:#000;border-radius:10%;-webkit-user-select:none;user-select:none}.grid-cell:hover{opacity:.8;background-color:var(--selected-color)}#controls-container{background-color:#0c0c0c;display:flex;flex-direction:column;min-width:200px;width:80%;justify-self:center;padding:5px;justify-content:center;gap:10px}#controls{display:grid;border-radius:5px;grid-template-columns:repeat(auto-fit,minmax(50px,1fr))}.color-cell{aspect-ratio:1}.color-cell:hover{border:solid 2px #ffffff}.controls-btn-container{display:grid;width:100%;grid-template-columns:repeat(2,1fr);gap:10px}.controls-btn{display:flex;width:5rem;justify-self:center;justify-content:center;width:90%}#load-input{display:none}
        </style>
    </head>
    <body>
        <header>
            <h1 class="title">ESPintar</h1>
        </header>

        <div id="content">
            <div id="controls-container">
                <div id="controls"></div>
                <div class="controls-btn-container">
                    <button id="eraser" class="controls-btn">Borrador</button>
                    <button id="clear" class="controls-btn">Limpiar</button>
                    <input
                        type="color"
                        id="color-picker"
                        class="controls-btn"
                    />
                </div>
                <div class="saveload-container">
                    <div id="save-btn">Save Grid</div>
                    <label for="load-input">Upload Grid</label>
                    <input
                        type="file"
                        id="load-input"
                        accept=".json"
                        name="load-input"
                    />
                </div>
            </div>
            <div id="grid-container">
                <div id="grid"></div>
            </div>
        </div>
        <script>
            (function(){const t=document.createElement("link").relList;if(t&&t.supports&&t.supports("modulepreload"))return;for(const r of document.querySelectorAll('link[rel="modulepreload"]'))n(r);new MutationObserver(r=>{for(const s of r)if(s.type==="childList")for(const c of s.addedNodes)c.tagName==="LINK"&&c.rel==="modulepreload"&&n(c)}).observe(document,{childList:!0,subtree:!0});function o(r){const s={};return r.integrity&&(s.integrity=r.integrity),r.referrerPolicy&&(s.referrerPolicy=r.referrerPolicy),r.crossOrigin==="use-credentials"?s.credentials="include":r.crossOrigin==="anonymous"?s.credentials="omit":s.credentials="same-origin",s}function n(r){if(r.ep)return;r.ep=!0;const s=o(r);fetch(r.href,s)}})();const g=new WebSocket("ws://"+window.location.hostname+":81/");g.onmessage=e=>{evtObj=json.parse(e.data),console.log(evtObj),E(evtObj)};const l=16,y=document.documentElement,a=document.getElementById("grid"),L=document.getElementById("controls");let d="";(()=>{for(let e=0;e<l*l;e++){const t=document.createElement("div");t.classList.add("grid-cell"),t.id=`cell-${e}`,a.appendChild(t),t.addEventListener("mousedown",()=>p(t)),t.addEventListener("mouseover",o=>{o.buttons===1&&p(t)})}})();const p=e=>{const t=parseInt(e.id.match(/\d+/)[0]),o=t%l,n=Math.floor(t/l);let r;switch(d){case"ERASER":e.style.backgroundColor="";default:e.style.backgroundColor=d}if(d=="ERASER"&&(r=[[o,n],[0,0,0]]),e.style.backgroundColor){let u=e.style.backgroundColor.slice(4,-1).split(", ").map(Number);var s=u[0],c=u[1],i=u[2];r=[[o,n],[s,c,i]]}g.send(JSON.stringify(r)),console.log(JSON.stringify(r))},b=e=>{const t=document.elementFromPoint(e.clientX,e.clientY);t.classList.contains("grid-cell")&&p(t)};a.addEventListener("touchstart",e=>{isDrawing=!0,b(e.touches[0])});a.addEventListener("touchmove",e=>{e.touches.length>=2||(e.preventDefault(),isDrawing&&b(e.touches[0]))});a.addEventListener("touchend",()=>{isDrawing=!1});const m={0:"#ffffff",1:"#232035",2:"#44293c",3:"#ff0000",4:"#df7027",5:"#d8a067",6:"#efc39b",7:"#fbf337",8:"#98e450",9:"#6abf31",10:"#37946e",11:"#4b682e",12:"#524b24",13:"#323c39",14:"#3e3f75",15:"#316083",16:"#5a6ee0",17:"#639afe",18:"#5ecde5",19:"#cadafc",20:"#dddddd",21:"#9badb7",22:"#857f87",23:"#686a6b"};(()=>{for(let e=0;e<24;e++){const t=document.createElement("div");t.classList.add("color-cell"),t.id=`color-${e}`,t.style.backgroundColor=m[e],t.addEventListener("mousedown",()=>{d=`${m[e]}`,y.style.setProperty("--selected-color",`${m[e]}`)}),L.appendChild(t)}})();const O=document.getElementById("eraser"),w=document.getElementById("clear"),C=document.getElementById("color-picker"),v=()=>{a.childNodes.forEach(t=>{t.style.backgroundColor=""});const e=JSON.stringify("LIMPIAR");g.send(e),console.log(e)};O.addEventListener("mousedown",()=>{d="ERASER",y.style.setProperty("--selected-color","#000000")});w.addEventListener("mousedown",v);C.addEventListener("input",e=>{d=e.target.value,y.style.setProperty("--selected-color",d)});const N=()=>{const e=[];let t,o,n=0;for(t=0;t<l;t++)for(o=0;o<l;o++){let i=a.childNodes[n].style.backgroundColor;if(i&&i!="rgb(0, 0, 0)"){let f=i.slice(4,-1).split(", ").map(Number);var r=f[0],s=f[1],c=f[2];const u=[[t,o],[r,s,c]];e.push(u)}n++}return JSON.stringify(e)},S=()=>{const e=N(),t=new Blob([e],{type:"application/json"}),o=URL.createObjectURL(t),n=document.createElement("a");n.href=o,n.download="gridData.json",n.style.display="none",document.body.appendChild(n),n.click(),document.body.removeChild(n),URL.revokeObjectURL(o)},D=document.getElementById("save-btn");D.addEventListener("click",S);const E=e=>{const t=JSON.parse(e);console.log(t),v(),t.forEach(o=>{let n=o[0][0]*l+o[0][1];a.childNodes[n].style.backgroundColor=`rgb(${o[1][0]}, ${o[1][1]}, ${o[1][2]})`})},I=e=>{const t=new FileReader;t.onload=o=>{const n=o.target.result;E(n),g.send(n)},t.readAsText(e)},R=document.getElementById("load-input");R.addEventListener("change",e=>{const t=e.target.files[0];t&&I(t)});
        </script>
    </body>
</html>)====" ;
