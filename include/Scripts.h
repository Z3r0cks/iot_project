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
const questions = $QUESTIONS;
for (const key in questions) {
    const quest = questions[key];
    const div = document.createElement("div");
    div.classList.add("question");
    div.id = key;
    
    div.innerHTML = quest.text;
    function clickHandler() {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                if (this.responseText.trim() == "$VALIDATION") {
                    div.removeEventListener("click", clickHandler);
                    window.location.reload();
                }
            }
        };
        xhttp.open("GET", "choose-question?question=" + key, true);
        xhttp.send();
    }
    div.addEventListener("click", clickHandler);
    document.querySelector("#cat" + questions[key].category).appendChild(div);
}
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

const char formIndexJS[] = R"=====(
const data = {};
const button = document.querySelector("#submit");
const question = document.querySelector("#question");
const a = document.querySelector("#answerA");
const b = document.querySelector("#answerB");
const c = document.querySelector("#answerC");
const d = document.querySelector("#answerD");
const form = document.querySelector("#form");
const error = document.querySelector("#error");
const success = document.querySelector("#success");

function validate() {
    data.question = question.value;
    data.a = a.value.trim();
    data.b = b.value.trim();
    data.c = c.value.trim();
    data.d = d.value.trim();
    return data.question.length > 0 && data.a > 0 && data.b > 0 && data.c > 0 && data.d > 0;
}

function submitHandler(event) {
    if (validate()) {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                if (this.responseText.trim() == "$VALIDATION") {
                    button.removeEventListener("click", submitHandler);
                    button.disabled = true;
                    success.classList.remove("hidden");
                }
            }
        };
        xhttp.open("GET", `submit-question?data=${encodeURIComponent(JSON.stringify(data))}&end=true`, true);
        xhttp.send();
    } else {
        error.classList.remove("hidden");
    }
}
button.addEventListener("click", submitHandler);
)=====";

const char formDownloadJS[] = R"=====(
const raw = "$DOWNLOAD";
if (raw) {
    const data = decodeURIComponent(raw);
    const file = new Blob([data], { type: "application/json" });
    const url = URL.createObjectURL(file);
    const a = document.createElement("a");
    a.href = url;
    a.download = "questions.json";
    a.innerText = "questions.json";
    document.body.appendChild(a);
    const questions = JSON.parse(data);
    for (const quest of questions) {
        const questDiv = document.createElement("div");
        questDiv.innerHTML = `
    <div>${quest.question}</div>
    <div>
        <div>${quest.a}</div>
        <div>${quest.b}</div>
        <div>${quest.c}</div>
        <div>${quest.d}</div>
    </div>`;
        document.body.appendChild(questDiv);
    }
}
)=====";