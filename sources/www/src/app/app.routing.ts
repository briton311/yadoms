import { Routes, RouterModule } from '@angular/router';

export const ROUTES: Routes = [
  { path: '', redirectTo:'home', pathMatch:'full'},
  //{ path: 'home', loadChildren: './home/home.module#HomeModule' }
  { path: 'home', loadChildren: "./home/home.module#HomeModule" },
  { path: 'dashboard', loadChildren: "./dashboard/dashboard.module#DashboardModule" },

    /*
  { path: '', redirectTo:'check', pathMatch:'full'},
  { path: 'check', component: CheckServerComponent},
  { path: 'home', loadChildren: './home/home.module#HomeModule' },
  { path: 'login', component: LoginComponent },
  { path: '**',    component: NoContent },
  */
];