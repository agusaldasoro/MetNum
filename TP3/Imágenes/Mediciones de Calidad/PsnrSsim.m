img = imread('forestAndWater.bmp');
img = imcrop(img, [1, 1, 499, 373]);
vec = imread('forestAndWater-vecino.bmp');
vec2 = imread('forestAndWater-vecino2.bmp');

vec = imcrop(vec, [1, 1, 499, 374]);
vec2 = imcrop(vec, [1, 1, 499, 374]);

psnr1 = psnr(vec, img);
psnr2 = psnr(vec2, img);

ssim1 = ssim(vec, img);
ssim2 = ssim(vec2, img);

psnr1
psnr2
ssim1
ssim2


%% 

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