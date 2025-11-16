/*
 ====================================================================================================
 * File:        styles.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.1.0
 * Date:        Nov 13 2025
 * Brief:       This File Contains CSS Styles For OTADash Web Pages
 * 
 ====================================================================================================
 * License: 
 * MIT License
 * 
 * Copyright (c) 2025 Hamas Saeed
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * For any inquiries, contact Hamas Saeed at hamasaeed@gmail.com
 *
 ====================================================================================================
 */

#ifndef WEBPAGES_STYLES_H
#define WEBPAGES_STYLES_H

#ifndef WEBPAGES_TEXT_COLOR
  #define WEBPAGES_TEXT_COLOR             "#ffffff"                           // Default text color
#endif

#ifndef WEBPAGES_PRIMARY_COLOR
  #define WEBPAGES_PRIMARY_COLOR          "#006064"                           // Default primary color
#endif

#ifndef WEBPAGES_ACCENT_COLOR
  #define WEBPAGES_ACCENT_COLOR           "#004d40"                           // Default accent color
#endif

#ifndef WEBPAGES_ACCENT_1_COLOR
  #define WEBPAGES_ACCENT_1_COLOR         "#00838f"                           // Default accent 1 color
#endif

#ifndef WEBPAGES_BACKGROUND_COLOR
  #define WEBPAGES_BACKGROUND_COLOR       "#e0f7fa"                           // Default background color
#endif

#ifndef WEBPAGES_BACKGROUND_2_COLOR
  #define WEBPAGES_BACKGROUND_2_COLOR     "#ffffff"                           // Default background 2 color
#endif

#ifndef WEBPAGES_BACKGROUND_3_COLOR
  #define WEBPAGES_BACKGROUND_3_COLOR     "#f2f2f2"                           // Default background 3 color
#endif

#ifndef WEBPAGES_BACKGROUND_4_COLOR
  #define WEBPAGES_BACKGROUND_4_COLOR     "#1e1e1e"                           // Default background 4 color
#endif

#ifndef WEBPAGES_BUTTON_COLOR
  #define WEBPAGES_BUTTON_COLOR           "#00838f"                           // Default button background color
#endif

#ifndef WEBPAGES_BUTTON_TEXT_COLOR
  #define WEBPAGES_BUTTON_TEXT_COLOR      "#ffffff"                           // Default button text color
#endif


#ifndef WEBPAGES_BORDER_COLOR
  #define WEBPAGES_BORDER_COLOR           "#ddd"                              // Default border color
#endif

#ifndef WEBPAGES_HEADER_FOOTER_COLOR
  #define WEBPAGES_HEADER_FOOTER_COLOR    "#666"                              // Default header/footer color
#endif

#ifndef WEBPAGES_LINK_COLOR
  #define WEBPAGES_LINK_COLOR             "#00838f"                           // Default link color
#endif

static const char otadash_css[] = R"rawliteral(
    body {
      color:              )rawliteral" WEBPAGES_PRIMARY_COLOR R"rawliteral(;
      margin:             0;
      padding:            0;
      display:            flex;
      text-align:         center;
      font-family:        "Arial";
      align-items:        center;
      justify-content:    center;
      background-color:   )rawliteral" WEBPAGES_BACKGROUND_COLOR R"rawliteral(;
    }

    .container {
      width:              90%;
      max-width:          500px;
      min-width:          300px;
      height:             auto;
      padding:            20px;
      display:            flex;
      margin-top:         50px;
      background:         )rawliteral" WEBPAGES_BACKGROUND_2_COLOR R"rawliteral(;
      box-shadow:         0 0 10px rgba(0, 0, 0, 0.1);
      align-items:        center;
      border-radius:      10px;
      flex-direction:     column;
    }

    .button {
      color:              )rawliteral" WEBPAGES_BUTTON_TEXT_COLOR R"rawliteral(;
      min-width:          160px;
      width:              auto;
      height:             auto;
      cursor:             pointer;
      border:             none;
      margin:             10px 8px;
      padding:            12px 20px;
      display:            inline-block;
      font-size:          16px;
      text-align:         center;
      border-radius:      5px;
      text-decoration:    none;
      background-color:   )rawliteral" WEBPAGES_BUTTON_COLOR R"rawliteral(;
      box-sizing:         border-box;
    }

    .button-container {
      gap:                10px;
      width:              100%;
      display:            flex;
      flex-wrap:          wrap;
      justify-content:    center;
      align-items:        center;
    }

    .note {
      color:              )rawliteral" WEBPAGES_ACCENT_COLOR R"rawliteral(;
      font-size:          14px;
      margin-top:         10px;
    }

    .note h5 {
      margin-bottom:      5px;
    }

    .separator {
      width:              100%;
      height:             1px;
      margin:             20px 0px;
      background-color:   )rawliteral" WEBPAGES_ACCENT_COLOR R"rawliteral(;
    }

    .footer {
      color:              )rawliteral" WEBPAGES_HEADER_FOOTER_COLOR R"rawliteral(;
      font-size:          11px;
      margin-top:         20px;
      border-top:         1px solid )rawliteral" WEBPAGES_BORDER_COLOR R"rawliteral(;
      padding-top:        15px;
    }

    .footer a {
      color:              )rawliteral" WEBPAGES_LINK_COLOR R"rawliteral(;
      text-decoration:    none;
    }

    .footer a:hover {
      text-decoration:    underline;
    }

    .content {
      text-align:         left;
      margin:             20px 0;
      font-size:          14px;
      line-height:        1.8;
    }

    .header {
      text-align:         center;
      margin-bottom:      20px;
    }

    .header h1 {
      margin:             0;
      color:              )rawliteral" WEBPAGES_ACCENT_COLOR R"rawliteral(;
    }
      
    .header p {
      margin:             5px 0;
      font-size:          12px;
      color:              )rawliteral" WEBPAGES_HEADER_FOOTER_COLOR R"rawliteral(;
    }

    .credits {
      background-color:   )rawliteral" WEBPAGES_BACKGROUND_3_COLOR R"rawliteral(;
      padding:            15px;
      border-radius:      5px;
      margin:             15px 0;
    }

    .credits h3 {
      margin-top:         0;
      color:              )rawliteral" WEBPAGES_ACCENT_COLOR R"rawliteral(;
      font-size:          14px;
    }

    .credits p {
      margin:             8px 0;
      font-size:          13px;
    }

    .credits a {
      color:              )rawliteral" WEBPAGES_LINK_COLOR R"rawliteral(;
      text-decoration:    none;
    }

    .credits a:hover {
      text-decoration:    underline;
    }

    table {
      width:              100%;
      margin:             20px 0;
      table-layout:       fixed;
      border-collapse:    collapse;
    }

    th, td {
      border:             1px solid )rawliteral" WEBPAGES_BORDER_COLOR R"rawliteral(;   
      padding:            10px;                                                         
      word-wrap:          break-word;
      word-break:         break-word;
      text-align:         left;
      white-space:        normal;
    }

    th {
      text-align:         center;
      background-color:   )rawliteral" WEBPAGES_BACKGROUND_3_COLOR R"rawliteral(;      
    }

    tr {
      height:             50px;
    }

    .wifi-list {
      width:              100%;
      padding:            10px;
      max-height:         250px;
      overflow-y:         auto;
      margin-top:         20px;
      box-shadow:         0 2px 4px rgba(0, 0, 0, 0.1);
      margin-bottom:      30px;
      border-radius:      8px;
      background-color:   )rawliteral" WEBPAGES_BACKGROUND_COLOR R"rawliteral(;
    }

    .wifi-item {
      cursor:             pointer;
      padding:            10px;
      border-bottom:      1px solid )rawliteral" WEBPAGES_BORDER_COLOR R"rawliteral(;
    }

    .wifi-item:last-child {
      border-bottom:      none;
    }
    .wifi-item:hover {
      background-color:   )rawliteral" WEBPAGES_BACKGROUND_3_COLOR R"rawliteral(;
    }

    h1 {
      font-size:          24px;
      margin-bottom:      20px;
    }
      
    .form-group {
      width:              100%;
      margin-bottom:      15px;
      display:            flex;
      flex-direction:     column;
    }
      
    label {
      color:              )rawliteral" WEBPAGES_ACCENT_COLOR R"rawliteral(;
      display:            block;
      font-size:          16px;
      text-align:         left;
      margin-bottom:      5px;
    }

    input[type="text"], input[type="password"] {
      width:              100%;
      border:             1px solid )rawliteral" WEBPAGES_BORDER_COLOR R"rawliteral(;
      padding:            8px;
      box-sizing:         border-box;
      border-radius:      4px;
      margin-bottom:      10px;
    }

    input[type="submit"] {
      cursor:             pointer;
      margin:             20px 5px;
      border:             none;
      padding:            12px 20px;
      font-size:          16px;
      border-radius:      5px;
      background-color:   )rawliteral" WEBPAGES_ACCENT_1_COLOR R"rawliteral(;
      color:              )rawliteral" WEBPAGES_TEXT_COLOR R"rawliteral(;
      text-decoration:    none;
      display:            inline-block;
      min-width:          140px;
      width:              auto;
      height:             auto;
      text-align:         center;
      box-sizing:         border-box;
    }

    input[type="file"] {
      padding:            10px;
      border:             2px dashed )rawliteral" WEBPAGES_ACCENT_1_COLOR R"rawliteral(;
      border-radius:      5px;
      width:              200px;
      cursor:             pointer;
    }

    #progressContainer {
      width:              100%;
      border:             2px solid )rawliteral" WEBPAGES_ACCENT_1_COLOR R"rawliteral(;
      display:            none;
      position:           relative;
      margin-top:         20px;
      border-radius:      5px;
    }

    #progressBar {
      width:              0%;
      height:             20px;
      position:           relative;
      transition:         width 0.4s ease;
      border-radius:      5px;
      background-color:   )rawliteral" WEBPAGES_ACCENT_1_COLOR R"rawliteral(;
    }

    #progressText {
      color:              )rawliteral" WEBPAGES_TEXT_COLOR R"rawliteral(;
      top:                0;
      left:               0;
      width:              100%;
      position:           absolute;
      text-align:         center;
      font-weight:        bold;
      line-height:        20px;
    }

    .log-screen {
      border:             1px solid )rawliteral" WEBPAGES_BORDER_COLOR R"rawliteral(";
      width:              400px;
      height:             400px;
      overflow-y:         scroll;
      padding:            10px;
      text-align:         left;
      background-color:   )rawliteral" WEBPAGES_BACKGROUND_4_COLOR R"rawliteral(;
      color:              )rawliteral" WEBPAGES_TEXT_COLOR R"rawliteral(;
      box-sizing:         border-box;
      font-family:        monospace;
      font-size:          12px;
      word-wrap:          break-word;
      white-space:        pre-wrap;
    }

    .erase-submit {
      background-color:   #d32f2f !important;
    }
    
    .erase-submit:hover {
      background-color:   #b71c1c !important;
    }
    
    .restart-submit {
      background-color:   #f09c3d !important;
    }
    
    .restart-submit:hover {
      background-color:   #e68900 !important;
    }
    
    @media (max-width: 480px) {
      .container {
        width: 95%;
        margin-top: 20px;
        padding: 15px;
      }
      
      .button {
        min-width: 160px;
        font-size: 14px;
        padding: 10px 15px;
        margin: 8px 3px;
      }
      
      input[type="submit"] {
        min-width: 160px;
        font-size: 14px;
        padding: 10px 15px;
      }
      
      .log-screen {
        height:             300px;
        background-color:   )rawliteral" WEBPAGES_BACKGROUND_4_COLOR R"rawliteral(;
        color:              )rawliteral" WEBPAGES_TEXT_COLOR R"rawliteral(;
        word-wrap:          break-word;
        white-space:        pre-wrap;
      }
      
      h1 {
        font-size: 20px;
      }
    }
)rawliteral";

#endif // WEBPAGES_STYLES_H