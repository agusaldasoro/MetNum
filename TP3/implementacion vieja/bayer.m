function [raw] = bayer(img)

r = img;
g = img;
b = img;

r(:,:,2) = 0;
r(:,:,3) = 0;

g(:,:,1) = 0;
g(:,:,3) = 0;

b(:,:,1) = 0;
b(:,:,2) = 0;

for i=1:size(r,1)
    if mod(i,2) == 0
        for j=1:size(r,2)
            b(i,j,3) = 0;
            if mod(j,2) == 0
                g(i,j,2) = 0;
            else
                r(i,j,1) = 0;
            end
        end
    else
        for j=1:size(r,2)
            r(i,j,1) = 0;
            if mod(j,2) == 0
                b(i,j,3) = 0;
            else
                g(i,j,2) = 0;
            end
        end
    end
end

raw = r+g+b;