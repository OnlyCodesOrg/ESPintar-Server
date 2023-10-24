String pagina = R"====(<!DOCTYPE html>
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
            (function(){const e=document.createElement("link").relList;if(e&&e.supports&&e.supports("modulepreload"))return;for(const n of document.querySelectorAll('link[rel="modulepreload"]'))r(n);new MutationObserver(n=>{for(const s of n)if(s.type==="childList")for(const c of s.addedNodes)c.tagName==="LINK"&&c.rel==="modulepreload"&&r(c)}).observe(document,{childList:!0,subtree:!0});function o(n){const s={};return n.integrity&&(s.integrity=n.integrity),n.referrerPolicy&&(s.referrerPolicy=n.referrerPolicy),n.crossOrigin==="use-credentials"?s.credentials="include":n.crossOrigin==="anonymous"?s.credentials="omit":s.credentials="same-origin",s}function r(n){if(n.ep)return;n.ep=!0;const s=o(n);fetch(n.href,s)}})();const g=new WebSocket("ws://"+window.location.hostname+":81/"),d=16,p=document.documentElement,a=document.getElementById("grid"),L=document.getElementById("controls");let i="";(()=>{for(let t=0;t<d*d;t++){const e=document.createElement("div");e.classList.add("grid-cell"),e.id=`cell-${t}`,a.appendChild(e),e.addEventListener("mousedown",()=>b(e)),e.addEventListener("mouseover",o=>{o.buttons===1&&b(e)})}})();g.onmessage=t=>{console.log(t);const e=t.data;if(console.log(e),Array.isArray(e[0][0])){v(e);return}if(Array.isArray(e[0])){let o=e[0][1],n=e[0][0]*d+o,s=e[1][0],c=e[1][1],l=e[1][2];console.log(n);const u=document.getElementById(`cell-${n}`);console.log(u),u.style.backgroundColor=`rgb(${s}, ${c}, ${l})`}e=="LIMPIAR"&&a.childNodes.forEach(o=>{o.style.backgroundColor=""})};const b=t=>{let e,o,r;const n=parseInt(t.id.match(/\d+/)[0]);switch(e=n%d,o=Math.floor(n/d),i){case"ERASER":t.style.backgroundColor="";default:t.style.backgroundColor=i}if(i=="ERASER"&&(r=[[e,o],[0,0,0]]),t.style.backgroundColor){let l=t.style.backgroundColor.slice(4,-1).split(", ").map(Number),u=l[0],f=l[1],m=l[2];r=[[e,o],[u,f,m]]}g.send(JSON.stringify(r)),console.log(JSON.stringify(r))},E=t=>{const e=document.elementFromPoint(t.clientX,t.clientY);e.classList.contains("grid-cell")&&b(e)};a.addEventListener("touchstart",t=>{isDrawing=!0,E(t.touches[0])});a.addEventListener("touchmove",t=>{t.touches.length>=2||(t.preventDefault(),isDrawing&&E(t.touches[0]))});a.addEventListener("touchend",()=>{isDrawing=!1});const y={0:"#ffffff",1:"#232035",2:"#44293c",3:"#ff0000",4:"#df7027",5:"#d8a067",6:"#efc39b",7:"#fbf337",8:"#98e450",9:"#6abf31",10:"#37946e",11:"#4b682e",12:"#524b24",13:"#323c39",14:"#3e3f75",15:"#316083",16:"#5a6ee0",17:"#639afe",18:"#5ecde5",19:"#cadafc",20:"#dddddd",21:"#9badb7",22:"#857f87",23:"#686a6b"};(()=>{for(let t=0;t<24;t++){const e=document.createElement("div");e.classList.add("color-cell"),e.id=`color-${t}`,e.style.backgroundColor=y[t],e.addEventListener("mousedown",()=>{i=`${y[t]}`,p.style.setProperty("--selected-color",`${y[t]}`)}),L.appendChild(e)}})();const C=document.getElementById("eraser"),w=document.getElementById("clear"),I=document.getElementById("color-picker"),h=()=>{const t=JSON.stringify("LIMPIAR");g.send(t),console.log(t),a.childNodes.forEach(e=>{e.style.backgroundColor=""})};C.addEventListener("mousedown",()=>{i="ERASER",p.style.setProperty("--selected-color","#000000")});w.addEventListener("mousedown",h);I.addEventListener("input",t=>{i=t.target.value,p.style.setProperty("--selected-color",i)});const N=()=>{const t=[];let e,o,r=0;for(e=0;e<d;e++)for(o=0;o<d;o++){let l=a.childNodes[r].style.backgroundColor;if(l&&l!="rgb(0, 0, 0)"){let f=l.slice(4,-1).split(", ").map(Number);var n=f[0],s=f[1],c=f[2];const m=[[e,o],[n,s,c]];t.push(m)}r++}return JSON.stringify(t)},O=()=>{const t=N(),e=new Blob([t],{type:"application/json"}),o=URL.createObjectURL(e),r=document.createElement("a");r.href=o,r.download="gridData.json",r.style.display="none",document.body.appendChild(r),r.click(),document.body.removeChild(r),URL.revokeObjectURL(o)},D=document.getElementById("save-btn");D.addEventListener("click",O);const v=t=>{const e=JSON.parse(t);console.log(e),h(),e.forEach(o=>{let r=o[0][0]*d+o[0][1];a.childNodes[r].style.backgroundColor=`rgb(${o[1][0]}, ${o[1][1]}, ${o[1][2]})`})},S=t=>{const e=new FileReader;e.onload=o=>{const r=o.target.result;v(r),g.send(r)},e.readAsText(t)},A=document.getElementById("load-input");A.addEventListener("change",t=>{const e=t.target.files[0];e&&S(e)});
        </script>
    </body>
</html>)====";
