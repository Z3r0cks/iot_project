const questions = $QUESTIONS;
for (const key in questions) {
    const quest = questions[key];
    const button = document.createElement("button");
    button.classList.add("question");
    button.id = key;
    
    button.innerHTML = quest.text;
    function clickHandler() {
        console.log(key);
        button.classList.add("answerClicked");
        const xhttp = new XMLHttpRequest();
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
    }
    button.addEventListener("click", clickHandler);
    document.querySelector("#cat" + questions[key].category).appendChild(button);
}