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
            (function(){const e=document.createElement("link").relList;if(e&&e.supports&&e.supports("modulepreload"))return;for(const n of document.querySelectorAll('link[rel="modulepreload"]'))r(n);new MutationObserver(n=>{for(const s of n)if(s.type==="childList")for(const c of s.addedNodes)c.tagName==="LINK"&&c.rel==="modulepreload"&&r(c)}).observe(document,{childList:!0,subtree:!0});function o(n){const s={};return n.integrity&&(s.integrity=n.integrity),n.referrerPolicy&&(s.referrerPolicy=n.referrerPolicy),n.crossOrigin==="use-credentials"?s.credentials="include":n.crossOrigin==="anonymous"?s.credentials="omit":s.credentials="same-origin",s}function r(n){if(n.ep)return;n.ep=!0;const s=o(n);fetch(n.href,s)}})();const f=new WebSocket("ws://"+window.location.hostname+":81/");f.onmessage=t=>{if(evtJSON=json.parse(t.data),console.log(evtJSON),Array.isArray(evtJSON[0][0]))h(evtJSON);else if(Array.isArray(evtJSON[0])){let e=cell[0][0],o=cell[0][1],r=e*d+o,n=cell[1][0],s=cell[1][1],c=cell[1][2];const l=document.getElementById(`cell-${r}`);l.style.backgroundColor=`rgb(${n}, ${s}, ${c})`}else evtJSON=="LIMPIAR"&&a.childNodes.forEach(e=>{e.style.backgroundColor=""})};const d=16,b=document.documentElement,a=document.getElementById("grid"),L=document.getElementById("controls");let i="";(()=>{for(let t=0;t<d*d;t++){const e=document.createElement("div");e.classList.add("grid-cell"),e.id=`cell-${t}`,a.appendChild(e),e.addEventListener("mousedown",()=>y(e)),e.addEventListener("mouseover",o=>{o.buttons===1&&y(e)})}})();const y=t=>{let e,o,r;const n=parseInt(t.id.match(/\d+/)[0]);switch(e=n%d,o=Math.floor(n/d),i){case"ERASER":t.style.backgroundColor="";default:t.style.backgroundColor=i}if(i=="ERASER"&&(r=[[e,o],[0,0,0]]),t.style.backgroundColor){let l=t.style.backgroundColor.slice(4,-1).split(", ").map(Number),p=l[0],u=l[1],g=l[2];r=[[e,o],[p,u,g]]}f.send(JSON.stringify(r)),console.log(JSON.stringify(r))},v=t=>{const e=document.elementFromPoint(t.clientX,t.clientY);e.classList.contains("grid-cell")&&y(e)};a.addEventListener("touchstart",t=>{isDrawing=!0,v(t.touches[0])});a.addEventListener("touchmove",t=>{t.touches.length>=2||(t.preventDefault(),isDrawing&&v(t.touches[0]))});a.addEventListener("touchend",()=>{isDrawing=!1});const m={0:"#ffffff",1:"#232035",2:"#44293c",3:"#ff0000",4:"#df7027",5:"#d8a067",6:"#efc39b",7:"#fbf337",8:"#98e450",9:"#6abf31",10:"#37946e",11:"#4b682e",12:"#524b24",13:"#323c39",14:"#3e3f75",15:"#316083",16:"#5a6ee0",17:"#639afe",18:"#5ecde5",19:"#cadafc",20:"#dddddd",21:"#9badb7",22:"#857f87",23:"#686a6b"};(()=>{for(let t=0;t<24;t++){const e=document.createElement("div");e.classList.add("color-cell"),e.id=`color-${t}`,e.style.backgroundColor=m[t],e.addEventListener("mousedown",()=>{i=`${m[t]}`,b.style.setProperty("--selected-color",`${m[t]}`)}),L.appendChild(e)}})();const N=document.getElementById("eraser"),O=document.getElementById("clear"),S=document.getElementById("color-picker"),E=()=>{a.childNodes.forEach(e=>{e.style.backgroundColor=""});const t=JSON.stringify("LIMPIAR");f.send(t),console.log(t)};N.addEventListener("mousedown",()=>{i="ERASER",b.style.setProperty("--selected-color","#000000")});O.addEventListener("mousedown",E);S.addEventListener("input",t=>{i=t.target.value,b.style.setProperty("--selected-color",i)});const C=()=>{const t=[];let e,o,r=0;for(e=0;e<d;e++)for(o=0;o<d;o++){let l=a.childNodes[r].style.backgroundColor;if(l&&l!="rgb(0, 0, 0)"){let u=l.slice(4,-1).split(", ").map(Number);var n=u[0],s=u[1],c=u[2];const g=[[e,o],[n,s,c]];t.push(g)}r++}return JSON.stringify(t)},w=()=>{const t=C(),e=new Blob([t],{type:"application/json"}),o=URL.createObjectURL(e),r=document.createElement("a");r.href=o,r.download="gridData.json",r.style.display="none",document.body.appendChild(r),r.click(),document.body.removeChild(r),URL.revokeObjectURL(o)},I=document.getElementById("save-btn");I.addEventListener("click",w);const h=t=>{const e=JSON.parse(t);console.log(e),E(),e.forEach(o=>{let r=o[0][0]*d+o[0][1];a.childNodes[r].style.backgroundColor=`rgb(${o[1][0]}, ${o[1][1]}, ${o[1][2]})`})},D=t=>{const e=new FileReader;e.onload=o=>{const r=o.target.result;h(r),f.send(r)},e.readAsText(t)},J=document.getElementById("load-input");J.addEventListener("change",t=>{const e=t.target.files[0];e&&D(e)});
        </script>
    </body>
</html>)====";
