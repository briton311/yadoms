import { Component } from '@angular/core';
import '../../public/css/bootstrap-non-responsive.css';
import '../../public/css/yadoms.css';
import '../../public/css/widget.css';
import '../../public/css/loading.css';

@Component({
  selector: 'my-app',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent { 
  private footerContent : string = "Copyright 2016 © Yadoms";
  private footerVersionContent : string = 'v 1.0.0 Beta';  
}
