function [raw] = bayersimple(img)

r = img(:,:,1);
g = img(:,:,2);
b = img(:,:,3);

for i=1:size(r,1)
    for j=1:size(r,2)
    	if mod(i,2) ~= 0
            r(i,j) = 0;
            if mod(j,2) == 0
                b(i,j) = 0;
            else
                g(i,j) = 0;
            end
        else
        	b(i,j) = 0;
            if mod(j,2) == 0
                g(i,j) = 0;
            else
                r(i,j) = 0;
            end
    	end
    end
end

raw = r+g+b;