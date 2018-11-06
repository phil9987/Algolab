for f in *.in; do echo "$f"; time ./flows < $f > ${f%.*}.myout; diff ${f%.*}.out ${f%.*}.myout;
done