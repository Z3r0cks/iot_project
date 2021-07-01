const char masterIndexJS[] = R"=====(
document.querySelector("#gamecode").innerHTML = "$KEY";
const interval = setInterval(function () {
    const xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            if (this.responseText.trim() == "$VALIDATION") {
                clearInterval(interval);
                window.location.reload();
            }
        }
    };
    xhttp.open("GET", "validate-key", true);
    xhttp.send();
}, 1000);
)=====";

const char masterQuestionsJS[] = R"=====(
const questions=$QUESTIONS;for(const e in questions){const t=questions[e],n=document.createElement("div");function clickHandler(){const t=new XMLHttpRequest;t.onreadystatechange=function(){4==this.readyState&&200==this.status&&"$VALIDATION"==this.responseText.trim()&&(n.removeEventListener("click",clickHandler),window.location.reload())},t.open("GET","choose-question?question="+e,!0),t.send()}n.classList.add("question"),n.id=e,n.innerHTML=t.text,n.addEventListener("click",clickHandler),document.querySelector("#cat"+questions[e].category).appendChild(n)}
)=====";

const char masterSolutionJS[] = R"=====(
const questions = $QUESTIONS;
document.querySelector("#solution").innerHTML = questions["$ID"].text;
document.querySelector("#wait-info").innerHTML = questions["$ID"].explanation;
const interval = setInterval(function () {
    const xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            if (this.responseText.trim() == "$VALIDATION") {
                clearInterval(interval);
                window.location.reload();
            }
        }
    };
    xhttp.open("GET", "validate-answer", true);
    xhttp.send();
}, 1000);
)=====";