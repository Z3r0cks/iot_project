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