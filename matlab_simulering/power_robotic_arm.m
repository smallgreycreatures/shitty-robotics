syms Falpha Fbeta Va Vb alpha1 alpha2

alpha=linspace(degtorad(40),degtorad(90),100); %alpha angle (known,but not really,tricky) ,may need to change the angles 
beta=linspace(degtorad(90),degtorad(160),100); 
Fk=5; %[N]
%beta1=pi/2-beta./2;
%beta_angle_180=2*beta1+beta;
%beta2=pi-beta1;
%alpha_angle_180=alpha+alpha1+alpha2;
%sinlaw_1=sin(alpha1)/180;
%sinlaw_2=sin(alpha2)/110; %sinlaw_1=sinlaw_2
%gamma=degtorad(315)-alpha-beta;
%lot_of_angles=gamma+alpha+beta+degtorad(225); %equal to 180*(5-2)
%angle_360=pi/4+alpha+beta+gamma;
L = 100; %lengd pa klo
actuator_max_force = 42; 

%%%%%%%%
%visual picture, no calculations
%three forces
%resulting_force=Falpha+Fbeta+Fk;
%quiver3(0,0,0,-3,3,4) %quiver3(0,0,0,Falpha(1), Falpha(2), Falpha(3)
%hold on                %should look like the above in the end 
%quiver3(0,0,0,2,4,4)  %resulting force remains
%quiver3(0,0,0,0,0,-5) %origin as reference for the three vectors
%quiver3(0,0,0,0,5,3) %resulting force
%hold off 
%legend('Falpha', 'Fbeta', 'Fk','Resulting force')
%title('Forces on our robotarm, [N]')
%xlabel('x-axis')
%ylabel('y-axis')
%zlabel('z-axis')

%%%%%%%%%%%%%%%%%

%visual of multiple forces
%[x,y]=meshgrid(alpha,beta); %possible angles for robot
range=linspace(-40,-15,100); 
%[u,v]=meshgrid(range);

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



%contour(u,v,,[0,0],'r'), hold on
%contour(u,v,,[0,0],'b'), grid, hold off
%legend('f','g')
Va=degtorad(135)-alpha1; %both should be matrixes
Vb=degtorad(225)+alpha+beta./2;
%[xx,yy]=meshgrid(alpha,beta);
%z=abs(actuator_max_force*sin(xx)+actuator_max_force*sin(yy))-Fk;%is this correct, i'm uncertain?
%mesh(xx,yy,z), hold on
%title('Result of Foreces')
%xlabel('\alpha,[rad]') %alpha
%ylabel('\beta,[rad]')%beta 
%zlabel('Uppåtriktad kraft,[F]') 

%%%%%%%%%%%%%%%%%
%[xx,yy]=meshgrid(Va,Vb);
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
size(s1+sa)
size(cos(Va))
size(da-d1)
size(sin(Va))
Mfalpha=42*((s1+sa).*cos(Va)-(da-d1).*sin(Va))
Mfbeta=65*((s1+s2-sb).*cos(Vb)-(d2+db).*sin(Va))
Mfk=(d2+d3)*Fk;
%want abs(Mfalpha+mfbeta)>abs(mfk) 
%%%%%%%%%%%%%%%%%%%
%momentum at end of arm, subplot all momentum?
final_force=(abs(Mfalpha + Mfbeta))./(d2+d3);
size(final_force);
[xx,yy]=meshgrid(alpha,beta);
z=meshgrid(final_force);%is this correct, i'm uncertain?
mesh(xx,yy,z), hold on
%title('yayy')%
xlabel('\alpha,[rad]') %alpha
ylabel('\beta,[rad]')%beta 
zlabel('Uppåtriktad kraft,[F]') 