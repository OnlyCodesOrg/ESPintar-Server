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
            (function(){const e=document.createElement("link").relList;if(e&&e.supports&&e.supports("modulepreload"))return;for(const o of document.querySelectorAll('link[rel="modulepreload"]'))n(o);new MutationObserver(o=>{for(const s of o)if(s.type==="childList")for(const d of s.addedNodes)d.tagName==="LINK"&&d.rel==="modulepreload"&&n(d)}).observe(document,{childList:!0,subtree:!0});function r(o){const s={};return o.integrity&&(s.integrity=o.integrity),o.referrerPolicy&&(s.referrerPolicy=o.referrerPolicy),o.crossOrigin==="use-credentials"?s.credentials="include":o.crossOrigin==="anonymous"?s.credentials="omit":s.credentials="same-origin",s}function n(o){if(o.ep)return;o.ep=!0;const s=r(o);fetch(o.href,s)}})();const g=new WebSocket("ws://"+window.location.hostname+":81/"),a=16,p=document.documentElement,c=document.getElementById("grid"),v=document.getElementById("controls");let i="";(()=>{for(let t=0;t<a*a;t++){const e=document.createElement("div");e.classList.add("grid-cell"),e.id=`cell-${t}`,c.appendChild(e),e.addEventListener("mousedown",()=>b(e)),e.addEventListener("mouseover",r=>{r.buttons===1&&b(e)})}})();g.onmessage=t=>{if(console.log(t),t.data=="LIMPIAR"){c.childNodes.forEach(r=>{r.style.backgroundColor=""});return}const e=JSON.parse(t.data);if(Array.isArray(e[0][0])){console.log("Matriz recibida..."),h(e);return}if(Array.isArray(e[0])){console.log("Pixel recibido...");let r=e[0][0],o=e[0][1]*a+r,s=e[1][0],d=e[1][1],l=e[1][2];const f=document.getElementById(`cell-${o}`);f.style.backgroundColor=`rgb(${s}, ${d}, ${l})`;return}};const b=t=>{let e,r,n;const o=parseInt(t.id.match(/\d+/)[0]);switch(e=o%a,r=Math.floor(o/a),i){case"ERASER":t.style.backgroundColor="";default:t.style.backgroundColor=i}if(i=="ERASER"&&(n=[[e,r],[0,0,0]]),t.style.backgroundColor){let l=t.style.backgroundColor.slice(4,-1).split(", ").map(Number),f=l[0],u=l[1],m=l[2];n=[[e,r],[f,u,m]]}g.send(JSON.stringify(n))},E=t=>{const e=document.elementFromPoint(t.clientX,t.clientY);e.classList.contains("grid-cell")&&b(e)};c.addEventListener("touchstart",t=>{isDrawing=!0,E(t.touches[0])});c.addEventListener("touchmove",t=>{t.touches.length>=2||(t.preventDefault(),isDrawing&&E(t.touches[0]))});c.addEventListener("touchend",()=>{isDrawing=!1});const y={0:"#ffffff",1:"#232035",2:"#44293c",3:"#ff0000",4:"#df7027",5:"#d8a067",6:"#efc39b",7:"#fbf337",8:"#98e450",9:"#6abf31",10:"#37946e",11:"#4b682e",12:"#524b24",13:"#323c39",14:"#3e3f75",15:"#316083",16:"#5a6ee0",17:"#639afe",18:"#5ecde5",19:"#cadafc",20:"#dddddd",21:"#9badb7",22:"#857f87",23:"#686a6b"};(()=>{for(let t=0;t<24;t++){const e=document.createElement("div");e.classList.add("color-cell"),e.id=`color-${t}`,e.style.backgroundColor=y[t],e.addEventListener("mousedown",()=>{i=`${y[t]}`,p.style.setProperty("--selected-color",`${y[t]}`)}),v.appendChild(e)}})();const L=document.getElementById("eraser"),C=document.getElementById("clear"),N=document.getElementById("color-picker"),w=()=>{const t=JSON.stringify("LIMPIAR");c.childNodes.forEach(e=>{e.style.backgroundColor=""}),g.send(t)};L.addEventListener("mousedown",()=>{i="ERASER",p.style.setProperty("--selected-color","#000000")});C.addEventListener("mousedown",w);N.addEventListener("input",t=>{i=t.target.value,p.style.setProperty("--selected-color",i)});const I=()=>{const t=[];let e,r,n=0;for(e=0;e<a;e++)for(r=0;r<a;r++){let l=c.childNodes[n].style.backgroundColor;if(l&&l!="rgb(0, 0, 0)"){let u=l.slice(4,-1).split(", ").map(Number);var o=u[0],s=u[1],d=u[2];const m=[[e,r],[o,s,d]];t.push(m)}n++}return JSON.stringify(t)},O=()=>{const t=I(),e=new Blob([t],{type:"application/json"}),r=URL.createObjectURL(e),n=document.createElement("a");n.href=r,n.download="gridData.json",n.style.display="none",document.body.appendChild(n),n.click(),document.body.removeChild(n),URL.revokeObjectURL(r)},S=document.getElementById("save-btn");S.addEventListener("click",O);const h=t=>{c.childNodes.forEach(e=>{e.style.backgroundColor=""}),t.forEach(e=>{let r=e[0][1]*a+e[0][0];c.childNodes[r].style.backgroundColor=`rgb(${e[1][0]}, ${e[1][1]}, ${e[1][2]})`})},D=t=>{const e=new FileReader;e.onload=r=>{const n=r.target.result,o=JSON.parse(n);h(o),g.send(n)},e.readAsText(t)},k=document.getElementById("load-input");k.addEventListener("change",t=>{const e=t.target.files[0];e&&D(e)});
        </script>
    </body>
</html>)====";
