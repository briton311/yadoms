// Angular
import '@angular/platform-browser';
import '@angular/platform-browser-dynamic';
import '@angular/core';
import '@angular/common';
import '@angular/http';
import '@angular/router';
// RxJS
import 'rxjs';
// Other vendors for example jQuery, Lodash or Bootstrap
// You can import js, ts, css, sass, ...

//Font-Awesome
require("font-awesome-webpack");

//jQuery
import 'jquery';

//boostrap
import 'bootstrap/dist/js/bootstrap';
import 'bootstrap/dist/css/bootstrap.min.css';

//momentJS
const moment = require('moment');
moment.locale(window.navigator.language);

console.log(moment().format('LLL'));


import 'jquery-ui/themes/base/all.css';