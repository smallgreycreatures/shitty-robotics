function F = root2d(x)
    global alpha_in_root;
    F(1) = alpha_in_root+x(1)+x(2)-pi; %x1=alpha1, x2=alpha2
    F(2) = sin(x(1))/180-sin(x(2))/110; 