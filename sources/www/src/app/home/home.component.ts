import { Component, OnInit } from '@angular/core';

@Component({
    selector: 'app-home',
    templateUrl: './home.component.html',
    styleUrls:['./home.component.css']
})
export class HomeComponent implements OnInit {
    constructor() { }

    ngOnInit() { 
        // main.js
        var Packery = require('packery');

        var pckry = new Packery( '.grid', {
        // options...
        });
    }
}