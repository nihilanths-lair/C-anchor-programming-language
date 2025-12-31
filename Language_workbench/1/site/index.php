<?php
  $txt = "txt";
  $exe = "exe";
  $file_name_1 = "Interpreter_C\$_0.x_";
  $file_name_2 = "VM_C\$_0.x_";
  $file_name_3 = "Compiler_C\$_0.x_";
  $nbsp_1 = "&nbsp;";
  $nbsp_2 = "&nbsp;&nbsp;";
  $nbsp_3 = "&nbsp;&nbsp;&nbsp;";
  $nbsp_4 = "&nbsp;&nbsp;&nbsp;&nbsp;";
  $nbsp_5 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
  $nbsp_6 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
  $nbsp_7 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
  $nbsp_8 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
  $nbsp_09 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
  $nbsp_10 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
  $nbsp_11 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
  $nbsp_12 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
  $nbsp_14 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
  $nbsp_17 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
?>
<!DOCTYPE html>
<html lang="ru">
<head>
  <link rel="shortcut icon" type="image/x-icon" href="favicon.ico">
  <meta charset="utf-8">
  <title>Главная</title>
  <style>
  div {
    font-family: "Courier New", "monospace";
    font-size: 13px;
  }
  
  /* Стили для summary, чтобы выглядел как select */
  .select-style {
    display: inline-block;/**/
    border: 1px solid #aaa;/**/
    border-radius: 3px;/**/
    padding: 0 20px 0 5px;/**/
    background: #f8f8f8;/**/
    cursor: pointer;/**/
    position: relative;/**/
    color: #333;/**/
    text-decoration: none;/**/
    font-weight: normal;/**/
    margin: 0 2px;/**/
    vertical-align: baseline;/**/
    line-height: 1.4;/**/
  }
  
  .select-style::after {
    content: '▼';
    position: absolute;
    right: 4px;
    top: 50%;
    transform: translateY(-50%);
    font-size: 10px;
    color: #666;
  }
  
  details[open] > summary > .select-style::after {
    content: '▲';
  }
  
  summary {
    list-style: none;
    cursor: pointer;
    display: inline;
  }
  
  summary::-webkit-details-marker {
    display: none;
  }
  
  /* Отключение стандартных стилей для стрелочек в details */
  details[open] > summary::after,
  details:not([open]) > summary::after {
    content: none !important;
  }
  </style>
</head>
<body style="background-color: #F0F0F0;">
  <center>
    <h3>
      Последнее обновление: 31/12/2025<br>
      /!\ Убедительная просьба дождаться окончания реконструкции сайта! Окончательный запуск сайта будет произведён в начале января 2026 года.
    </h3>
    <hr>
  </center>

<br><br>
<div style="text-align: center;">
  <div style="display: inline-block; text-align: left; margin: 0;">
    <b>
            Карта навигации<br>
            │<br>
            └─›Программное обеспечение<br>
   <?=$nbsp_3?>│<br>
   <?=$nbsp_3?>├─›<a href="https://ru.wikipedia.org/wiki/%D0%98%D0%BD%D1%82%D0%B5%D1%80%D0%BF%D1%80%D0%B5%D1%82%D0%B0%D1%82%D0%BE%D1%80" target="_blank">Интерпретатор C$</a><br>
   <?=$nbsp_3?>│<?=$nbsp_2?>│<br>
   <details style="margin-left: 23px;">
      <summary>│<?=$nbsp_2?>└─›<span class="select-style">Версия</span></summary>
               │                              <?=$nbsp_5?>│<br>
               │                              <?=$nbsp_5?>├─›<span class="select-style">0.X</span>🔒<br>
               │                              <?=$nbsp_5?>│<br>
               │                              <?=$nbsp_5?>└─›<span class="select-style">1.X</span>🔒<br>
               │<br>
   </details>

   <?=$nbsp_3?>│<br>
   <?=$nbsp_3?>├─›<a href="https://ru.wikipedia.org/wiki/%D0%92%D0%B8%D1%80%D1%82%D1%83%D0%B0%D0%BB%D1%8C%D0%BD%D0%B0%D1%8F_%D0%BC%D0%B0%D1%88%D0%B8%D0%BD%D0%B0" target="_blank">Виртуальная машина C$</a><br>
   <?=$nbsp_3?>│<?=$nbsp_2?>│<br>
   <details style="margin-left: 23px;">
      <summary>│<?=$nbsp_2?>└─›<span class="select-style">Версия</span></summary>
               │                              <?=$nbsp_5?>│<br>
               │                              <?=$nbsp_5?>├─›<span class="select-style">0.X</span>🔒<br>
               │                              <?=$nbsp_5?>│<br>
               │                              <?=$nbsp_5?>└─›<span class="select-style">1.X</span>🔒<br>
               │<br>
   </details>

   <?=$nbsp_3?>│<br>
   <?=$nbsp_3?>└─›<a href="https://ru.wikipedia.org/wiki/%D0%9A%D0%BE%D0%BC%D0%BF%D0%B8%D0%BB%D1%8F%D1%82%D0%BE%D1%80" target="_blank">Компилятор C$</a><br>
      <?=$nbsp_6?>│<br>
      <details style="margin-left: 46px;">
         <summary>└─›<span class="select-style">Версия</span></summary>
                                    <?=$nbsp_4?>│<br>
                           <details>
                           <summary><?=$nbsp_4?>├─›<span class="select-style">0.X</span></summary>
                                    <?=$nbsp_4?>│                 <?=$nbsp_2?>│<br>
                                            <details><!-- 0.0.X -->
                                            <summary><?=$nbsp_4?>│<?=$nbsp_3?>├─›<span class="select-style">0.0.X</span></summary>
                                    <?=$nbsp_4?>│                 <?=$nbsp_2?>│                 <?=$nbsp_2?>│<br>
                                    <?=$nbsp_4?>│                 <?=$nbsp_2?>│                 <?=$nbsp_2?>├─›0.0.0 🔰<br>
                                    <?=$nbsp_4?>│                 <?=$nbsp_2?>│                 <?=$nbsp_2?>│<?=$nbsp_2?>│<br>
                                    <?=$nbsp_4?>│                 <?=$nbsp_2?>│                 <?=$nbsp_2?>│<?=$nbsp_2?>├─›Beta-release [в разработке] 🎯<br>
                                    <?=$nbsp_4?>│                 <?=$nbsp_2?>│                 <?=$nbsp_2?>│<?=$nbsp_2?>│<br>
                                    <?=$nbsp_4?>│                 <?=$nbsp_2?>│                 <?=$nbsp_2?>│<?=$nbsp_2?>└─›Stable-release [после обката бэтки] 🔒<br>
                                    <?=$nbsp_4?>│                 <?=$nbsp_2?>│                 <?=$nbsp_2?>│<br>
                                    <?=$nbsp_4?>│                 <?=$nbsp_2?>│                 <?=$nbsp_2?>│<br>
                                    <?=$nbsp_4?>│                 <?=$nbsp_2?>│                 <?=$nbsp_2?>└─›0.0.1 [следующая версия] 🔒<br>
                                            </details>
                                    <?=$nbsp_4?>│                 <?=$nbsp_2?>│<br>
                                    <?=$nbsp_4?>│                 <?=$nbsp_2?>│<br>
                                            <details><!-- 0.1.X -->
                                            <summary><?=$nbsp_4?>│<?=$nbsp_3?>└─›<span class="select-style">0.1.X</span>🔒</summary>
                                            </details>
                                    <?=$nbsp_4?>│<br>
                           </details>
                                    <?=$nbsp_4?>│<br>
                           <details>
                           <summary><?=$nbsp_4?>└─›<span class="select-style">1.X</span>🔒</summary>
                           </details>
      </details>
    </b>
  </div>
</div>
<br><br>
<hr>
  <center>
    <copy>© Meltrune 2026</copy>
  </center>
</body>
</html>