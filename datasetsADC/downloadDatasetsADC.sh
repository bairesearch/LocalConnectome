https://codex.flywire.ai/api/download
download connections.csv.gz
download coordinates.csv.gz
download neurons.csv.gz
gzip -d connections.csv.gz
gzip -d coordinates.csv.gz
gzip -d neurons.csv.gz
dos2unix connections.csv
dos2unix coordinates.csv
dos2unix neurons.csv
