function graphinst(fname)

% Parámetros
temperaturaRefrig = -100;
temperaturaNoDef  = 0;

fileID = fopen(fname,'r');
%formatSpec = '%f %f %d %d %f %f %d';
formatSpec = '%f %f %f %f %f %d';
vals = fscanf(fileID,formatSpec,[6 1]);



a = vals(2); %m = vals(4); % eje Y (filas)
b = vals(1); %n = vals(3); % eje X (columnas)
h = vals(3)
r = vals(4); % radio
t = vals(5); % temp de las sanguijuelas
k = vals(6); %
n = b/h
m = a/h

formatSpec = '%f %f';
sanguijuelas = fscanf(fileID,formatSpec,[2 Inf]);
sanguijuelas = sanguijuelas';
                                      
% Gráfico básico del parabrisas y las sanguijuelas
hold off;
plot(sanguijuelas(:,1),sanguijuelas(:,2),'*r','MarkerSize',20);
hold on;
set(gca, 'XTick', 0:(b/n):b);
set(gca, 'XLim', [0 b]);
set(gca, 'YTick', 0:(a/m):a);
set(gca, 'YLim', [0 a+10e-16]);
title('Parabrisas y Sanguijuelas');
xlabel('x'); ylabel('y');
grid on;
for y=0:(a/m):a,
    plot(0:(b/n):b,y,'.k','MarkerSize',20);
end

[x_cyl,y_cyl,z_cyl] = cylinder(r,200);

for i=1:k,
    plot(x_cyl(1,:)+sanguijuelas(i,1),y_cyl(1,:)+sanguijuelas(i,2),'r','LineWidth',2);
end

% Matrices de discretización
[X,Y] = meshgrid(0:(b/n):b,0:(a/m):a); % T(i,j) corresponde a la temperatura en el punto [X(i,j),Y(i,j)] 
T = zeros(m+1,n+1) + temperaturaNoDef; % matriz de temperaturas en cada punto (de la discretización). Se inicializa en una temperatura no usada.

% completo con datos conocidos:
for i=1:m+1,                                                               % recordar que se indexa en 1 en matlab
    for j=1:n+1,                                                           % recordar que se indexa en 1 en matlab
        % ver si es influido por alguna sanguijuela:
        for s=1:k,
            if norm(sanguijuelas(s,:)-[X(i,j),Y(i,j)],2) <= r, % el punto cae en el círculo que afecta la sanguijuela
                T(i,j) = t;
                plot(X(i,j),Y(i,j),'.r','MarkerSize',20);
            end
        end
    end
end
for i=1:m+1,                                                               % recordar que se indexa en 1 en matlab
    T(i,1  ) = temperaturaRefrig;
    T(i,n+1) = temperaturaRefrig;
    plot(X(i,1)  ,Y(i,1)  ,'.b','MarkerSize',20);
    plot(X(i,n+1),Y(i,n+1),'.b','MarkerSize',20);
end
for j=1:n+1,                                                               % recordar que se indexa en 1 en matlab
    T(1  ,j) = temperaturaRefrig;
    T(m+1,j) = temperaturaRefrig;
    plot(X(1  ,j),Y(1  ,j),'.b','MarkerSize',20);
    plot(X(m+1,j),Y(m+1,j),'.b','MarkerSize',20);
end
end
