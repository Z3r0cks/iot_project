const char masterIndexJS[] = R"=====(
"use strict";
document.querySelector("#gamecode").innerHTML = "$KEY";
var interval = setInterval(function () {
    var xhttp = new XMLHttpRequest();
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
"use strict";
var _a;
var questions = $QUESTIONS;
var _loop_1 = function (key) {
    var quest = questions[key];
    var button = document.createElement("button");
    button.classList.add("question");
    button.id = key;
    button.innerHTML = quest.text;
    var clickHandler = function () {
        console.log(key);
        button.classList.add("answerClicked");
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                if (this.responseText.trim() == "$VALIDATION") {
                    button.removeEventListener("click", clickHandler);
                    window.location.reload();
                }
            }
        };
        xhttp.open("GET", "choose-question?question=" + key, true);
        xhttp.send();
    };
    button.addEventListener("click", clickHandler);
    (_a = document.querySelector("#cat" + questions[key].category)) === null || _a === void 0 ? void 0 : _a.appendChild(button);
};
for (var key in questions) {
    _loop_1(key);
}3
)=====";

const char masterSolutionJS[] = R"=====(
"use strict";
var questions = $QUESTIONS;
document.querySelector("#solution").innerHTML = questions["$ID"].text;
document.querySelector("#wait-info").innerHTML = questions["$ID"].explanation;
var interval = setInterval(function () {
    var xhttp = new XMLHttpRequest();
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
"use strict";
var data = {};
var button = document.querySelector("#submit");
var question = document.querySelector("#question");
var a = document.querySelector("#answerA");
var b = document.querySelector("#answerB");
var c = document.querySelector("#answerC");
var d = document.querySelector("#answerD");
var form = document.querySelector("#form");
var error = document.querySelector("#error");
var success = document.querySelector("#success");
function validate() {
    data.question = question.value;
    data.a = a.value.trim();
    data.b = b.value.trim();
    data.c = c.value.trim();
    data.d = d.value.trim();
    return data.question.length > 0 && data.a.length > 0 && data.b.length > 0 && data.c.length > 0 && data.d.length > 0;
}
function submitHandler() {
    if (validate()) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                if (this.responseText.trim() == "$VALIDATION") {
                    button.removeEventListener("click", submitHandler);
                    button.disabled = true;
                    success === null || success === void 0 ? void 0 : success.classList.remove("hidden");
                }
            }
        };
        xhttp.open("GET", "submit-question?data=" + encodeURIComponent(JSON.stringify(data)) + "&end=true", true);
        xhttp.send();
    }
    else {
        error === null || error === void 0 ? void 0 : error.classList.remove("hidden");
    }
}
button.addEventListener("click", submitHandler);
)=====";

const char formDownloadJS[] = R"=====(
"use strict";
var raw = "$DOWNLOAD";
if (raw) {
    var data = decodeURIComponent(raw);
    var file = new Blob([data], { type: "application/json" });
    var url = URL.createObjectURL(file);
    var a = document.createElement("a");
    a.href = url;
    a.download = "questions.json";
    a.innerText = "questions.json";
    document.body.appendChild(a);
    var questions = JSON.parse(data);
    for (var _i = 0, questions_1 = questions; _i < questions_1.length; _i++) {
        var quest = questions_1[_i];
        var questDiv = document.createElement("div");
        questDiv.innerHTML = "<div>" + quest.question + "</div><div><div>" + quest.a + "</div><div>" + quest.b + "</div><div>" + quest.c + "</div><div>" + quest.d + "</div></div>";
        document.body.appendChild(questDiv);
    }
}
)=====";