img = imread('img12.bmp');
imgVEC = imread('img12-vecino.bmp');
        %psnrVEC = psnr(imgVEC, img);
        %ssimVEC = ssim(imgVEC, img);
A = img - imgVEC;
        %A = imcomplement(A);
imshow(A)
        %imwrite(A, 'DIFFimg9VEC.jpeg')

%% 

img = imcrop(img, [2, 2, 765, 509]);
imgBI = imread('img12-bilineal.bmp');
        %psnrBI = psnr(imgBI, img);
        %ssimBI = ssim(imgBI, img);
B = img - imgBI;
        %B = imcomplement(B);
imshow(B)
        %imwrite(B, 'DIFFimg1BI.jpeg')

%% 

img = imcrop(img, [2, 2, 763, 507]);
imgMHC = imread('img12-MHC.bmp');
        %psnrMHC = ps        nr(imgMHC, img);
        %ssimMHC = ssim(imgMHC, img);
C = img - imgMHC;
        %C = imcomplement(C);
imshow(C)
        %imwrite(C, 'DIFFimg1MHC.jpeg')

%% 
psnrVEC
psnrBI 
psnrMHC 


ssimVEC
ssimBI 
ssimMHC 