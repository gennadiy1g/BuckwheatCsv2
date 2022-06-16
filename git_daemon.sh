# The bare repository is located at: /home/gennadiy/git/BuckwheatCsv.git
# The bare repository is located at: /home/gennadiy/git/CsvTable.git

# Remote repository URL: git://192.168.2.10/BuckwheatCsv.git
# Remote repository URL: git://192.168.2.10/CsvTable.git

# Add firewall rule: port 9418, from 10.0.2.0, to 10.0.2.255, both protocols, direction: In,
# all interfaces

git daemon --reuseaddr --verbose --base-path=/home/gennadiy/git --export-all --enable=receive-pack -- /home/gennadiy/git/BuckwheatCsv.git /home/gennadiy/git/CsvTable.git
