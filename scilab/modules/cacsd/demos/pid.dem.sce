//
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

function demo_pid()

  mode(-1);
  lines(0);
  //display the diagram
  x=[5,10,20,40,50,70,80,90];xmin=-10;xmax=100;
  y=[22,28,30,32];ymin=12;ymax=40;

  xx=[xmin,xmin,x([1 2 2 7 4 6 3 4 5 6 3 3 5 5]);xmax,xmax,x([3,2,7,7,5,8,3,4,5,6,4,4,6,6])];
  yy=[ymin,ymax,y([3,1,1,1,3,3,2,2,2,2,2,4,2,4]);ymin,ymax,y([3,3,1,3,3,3,4,4,4,4,2,4,2,4])];

  scf(100001);clf();show_window();
  plot2d(xx,yy,ones(1,16),'022');
  xstring(28,30,'K');xstring(56,30,'Plant');xstring(12,28.80,'-');
  xtitle('PLANT   and   CONTROLLER')
  mode(2);

  path=get_absolute_file_path('pid.dem.sce');
  s=poly(0,'s');z=poly(0,'z');
  messagebox(['Example of PID Design '
              'file: '+path+'pid.dem.sce'],"modal");

  n=x_choose(['Continuous time';'Discrete time'],'Select time domain');
  select n
   case 0
    warning('Demo stops!');return;
   case 1
    mode(1)
    dom='c';
    s=poly(0,'s');
    str='[(s-1)/(s^2+5*s+1)]';
    rep=x_dialog('Nominal plant?',str);
    if rep==[] then return,end
    Plant=evstr(rep);
    Plant=syslin('c',Plant);
    mode(-1)
  case 2
    mode(1)
    dom='d'
    z=poly(0,'z');
    str='(z+1)/(z^2-5*z+2)'
    rep=x_dialog('Nominal plant?',str);
    if rep==[] then return,end
    Plant=evstr(rep)
    Plant=syslin('d',Plant);
    mode(-1)
  end
  //Nominal Plant
  P22=tf2ss(Plant);    //...in state-space form
  [ny,nu,nx]=size(P22);
  defv=['-1.2';'1';'0.1'];
  if dom=='d' then defv=['-10';'1';'0.1'];end
  while %t
    mode(1)
    if dom=='c' then
      _title='Enter your PID controller K(s)=Kp*(1+T0/s+T1*s)';
    end
    if dom=='d' then
      _title='Enter your PID controller K(z)=Kp*(1+T0/z+T1*z)';
    end
    defv=x_mdialog(_title,['Kp=';'T0=';'T1='],defv);
    if defv==[] then warning('Demo stops!');return;end
    Kp=evstr(defv(1));T0=evstr(defv(2));T1=evstr(defv(3));
    if dom=='c' then
      Kpid=tf2ss(Kp*(1+T0/s+T1*s));
    end
    if dom=='d' then
      Kpid=tf2ss(Kp*(1+T0/z+T1*z));
    end
    W=[1, -P22;
        Kpid,1];Winv=inv(W);

    disp(spec(Winv(2)),'closed loop eigenvalues');//Check internal stability
    if max(real(spec(Winv(2)))) > 0 then
      messagebox('You lose: closed-loop is UNSTABLE!!!',"modal");
    else
      messagebox('Congratulations: closed-loop is STABLE !!!',"modal");
      break;
    end
    mode(-1)
  end
  mode(1)
  [Spid,Rpid,Tpid]=sensi(P22,Kpid);  //Sensitivity functions
  Tpid(5)=clean(Tpid(5));

  disp(clean(ss2tf(Spid)),'Sensitivity function');
  disp(clean(ss2tf(Tpid)),'Complementary sensitivity function');

  resp=['Frequency response';'Time response'];
  while %t do
    n=x_choose(resp,'Select response(s)');
    if degree(Tpid(5))>0 then
      warning('Improper transfer function! D(s) set to D(0)')
      Tpid(5)=coeff(Tpid(5),0);
    end
    Tpid(5)=coeff(Tpid(5));
    select n
    case 0
      break
    case 1
      mode(1)
      scf(100002);clf(100002);show_window();bode(Tpid);
      mode(-1)
    case 2
      if Plant(4)=='c' then
        mode(1)
        defv=['0.1';'50'];
        _title='Enter Sampling period and Tmax';
        rep=x_mdialog(_title,['Sampling period?';'Tmax?'],defv);
        if rep==[] then break,end
        dttmax=evstr(rep);
        dt=evstr(dttmax(1));tmax=evstr(dttmax(2));
        t=0:dt/5:tmax;
        n1=x_choose(['Step response?';'Impulse response?'],'Simulation:');
        if n1==0 then
          warning('Demo stops!');return;
        end
        if n1==1 then
          scf(100002);clf(100002);show_window();
          plot2d([t',t'],[(csim('step',t,Tpid))',ones(t')])
        end
        if n1==2 then
          scf(100002);clf(100002);show_window();
          plot2d([t',t'],[(csim('impul',t,Tpid))',0*t'])
        end
        mode(-1)
      elseif Plant(4)=='d' then
        mode(1)
        defv=['100'];
        _title='Tmax?'
        rep=x_mdialog(_title,['Tmax='],defv);
        if rep==[] then break,end
        Tmax=evstr(rep);
        mode(-1)
        while %t do
          n=x_choose(['Step response?';'Impulse response?'],'Simulation:');
          select n
          case 0 then
            break
          case 1 then
            mode(1)
            u=ones(1,Tmax);u(1)=0;
            scf(100002);clf(100002);show_window();
            plot2d([(1:Tmax)',(1:Tmax)'],[(dsimul(Tpid,u))',(ones(1:Tmax)')])
            mode(-1)
          case 2 then
            mode(1)
            u=zeros(1,Tmax);u(1)=1;
            scf(100002);clf(100002);show_window();
            plot2d((1:Tmax)',(dsimul(Tpid,u))')
            mode(-1)
          end
        end
      end
    end
  end
endfunction

demo_pid();
clear demo_pid;
