syms Falpha Fbeta Va Vb alpha1 alpha2

alpha=linspace(degtorad(40),degtorad(90),100); %alpha angle (known,but not really,tricky) ,may need to change the angles
beta=linspace(degtorad(90),degtorad(160),100);
Fk=5; %[N]
L = 100; %lengd pa klo
actuator_max_force = 42;

range=linspace(-40,-15,100);

%solvin' a non linear equation system
global alpha_in_root;
alpha_list=0;
for alp = alpha
   alpha_in_root = alp;

    fun = @root2d;
    x0 = [0 0]; %start at x0

    options = optimoptions('fsolve','Display','iter');
     x=fsolve(fun, x0, options);
     alpha_list=[alpha_list x];

end
alpha1=alpha_list(2:2:end-1); %first index(1) is of no interest
alpha2=alpha_list(3:2:end);



Va=degtorad(135)-alpha1; %both should be matrixes
Vb=degtorad(225)+alpha+beta./2;
[xx,yy]=meshgrid(alpha,beta);
z=abs(actuator_max_force*sin(xx)+actuator_max_force*sin(yy))-Fk;%is this correct, i'm uncertain?
%mesh(xx,yy,z), hold on
title('Result of Foreces')
xlabel('\alpha,[rad]') %alpha
ylabel('\beta,[rad]')%beta
zlabel('Uppåtriktad kraft,[F]')


%new variables
s1=100*sqrt(2);
s2=260*sin(alpha-pi/4);
s3=(180+L)*sin(beta+alpha-pi/4);
sa=180*sin(alpha-pi/4);
sb=80*sin(beta+alpha-pi/4);
d1=200*cos(pi/4);
d2=260*cos(alpha-pi/4);
d3=-(180+L)*cos(alpha+beta-pi/4);
da=180*cos(alpha-pi/4);
db=-80*cos(alpha+beta-pi/4);
%Momentum
size(s1+sa);
size(cos(Va));
size(da-d1);
size(sin(Va));
Mfalpha=42*((s1+sa).*cos(Va)-(da-d1).*sin(Va));
Mfbeta=42*((s1+s2-sb).*cos(Vb)-(d2+db).*sin(Va));
Mfk=(d2+d3)*Fk;
%want abs(Mfalpha+mfbeta)>abs(mfk)

%momentum at end of arm

final_force=(abs(Mfalpha + Mfbeta))./(d2+d3);
size(final_force);
z=meshgrid(final_force);
mesh(xx,yy,z), hold on
xlabel('\alpha,[rad]') %alpha
ylabel('\beta,[rad]')%beta
zlabel('Uppåtriktad kraft,[N]')
hold off
