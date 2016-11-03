import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { HomeComponent }   from './home.component';
import { routing }             from './home.routing';

@NgModule({
    imports: [CommonModule, routing],
    exports: [],
    declarations: [HomeComponent],
    providers: []
})
export class HomeModule {
    constructor() {
        console.log("Loading HomeModule");
    }
 }
