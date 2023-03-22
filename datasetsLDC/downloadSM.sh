wget https://www.science.org/doi/suppl/10.1126/science.add9330/suppl_file/science.add9330_data_s1_to_s4.zip
unzip science.add9330_data_s1_to_s4.zip
unzip science.add9330_data_s1.zip
mv Supplementary-Data-S1/*.csv .
rm *.zip
rm -rf Supplementary-Data-S1
rm -rf __MACOSX
