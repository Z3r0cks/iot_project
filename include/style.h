const char playerStyle[] = R"=====(
body {
   background-color: #436436;
   font-family: "Roboto", sans-serif;
   color: #555;
   font-size: 25px;
}
.wrapper {
   position: relative;
   max-width: 450px;
   margin: 0 auto;
   display: flex;
   flex-direction: column;
   padding: 0 20px;
}
.headerWrapper {
   background-color: #efefef;
   border-radius: 50px;
   padding: 25px;
   margin-top: 90px;
   margin-bottom: 50px;
   min-height: 120px;
   text-align: center;
}

.header {
   justify-content: center;
   display: flex;
   margin: 5rem 0;
}

.inputWrapper {
   display: flex;
   justify-content: center;
   background-color: #efefef;
   border-radius: 50px;
   padding: 25px;
   margin: 20px 90px;
}

input {
   border: none;
   text-align: center;
   background-color: #efefef;
   border-radius: 25px;
   font-size: 30px;
   margin: 40px 50px 40px 50px;
   color: inherit;
}

.btn {
   height: 100px;
   width: 100px;
   border-radius: 50px;
   border: none;
   margin: 0 auto;
   background-color: #7a8f72;
   color: white;
   font-size: 18px;
}

.indexheaderWrapper {
   margin-top: 32px;
   margin-bottom: 20px;
}

.indexHeader {
   margin-top: 20px !important;
}

.questionHeaderWrapper {
   margin-top: 20px !important;
   margin-bottom: 20px !important;
   padding: 0 25px !important;
}

.questionHeader {
   font-size: 20px;
}

.questionTitle {
   color: #efefef;
   margin-bottom: 2px;
}

.answerWrapperBlock {
   display: flex;
}

.oneAnswerWrapper {
   flex: 1;
}

.answerWrapper {
   background-color: #efefef;
   padding: 15px;
   min-height: 100px;
}

.margin-right {
   margin-right: 10px;
}

.margin-left {
   margin-left: 10px;
}

.waitHeaderWrapper {
   margin-top: 30px;
}

.waitHeader {
   margin: 0;
   padding: 0;
   margin-bottom: 35px;
}

.waitHeader:first-child {
   margin: 0;
   padding: 0;
   margin-top: 35px;
   margin-bottom: 35px;
}

.solutionHeaderWrapper {
   margin-top: 20px;
}
.contentWrapper {
   background-color: #efefef;
   text-align: center;
   font-size: 18px;
   padding: 7px;
   margin-bottom: 20px;
}
)=====";