import { NgModule } from '@angular/core';
import { BrowserModule }  from '@angular/platform-browser';
import { AppComponent } from './app.component';

//routing
import { RouterModule, Router } from '@angular/router';
import { ROUTES } from './app.routing';


@NgModule({
  imports: [
    BrowserModule,
    RouterModule.forRoot(ROUTES, { useHash: false /*, enableTracing:true */})
  ],
  declarations: [
    AppComponent
  ],
  bootstrap: [ AppComponent ]
})
export class AppModule { }
