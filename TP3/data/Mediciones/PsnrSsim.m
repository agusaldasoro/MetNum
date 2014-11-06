img = imread('img12.bmp');
imgVEC = imread('img12-vecino.bmp');
psnrVEC = psnr(imgVEC, img);
ssimVEC = ssim(imgVEC, img);
%% 

img = imcrop(img, [2, 2, 765, 509]);
imgBI = imread('img12-bilineal.bmp');
psnrBI = psnr(imgBI, img);
ssimBI = ssim(imgBI, img);
%% 

img = imcrop(img, [2, 2, 763, 507]);
imgMHC = imread('img12-MHC.bmp');
psnrMHC = psnr(imgMHC, img);
ssimMHC = ssim(imgMHC, img);

%% 
psnrVEC
psnrBI 
psnrMHC 


ssimVEC
ssimBI 
ssimMHC 