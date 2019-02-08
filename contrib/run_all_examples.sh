build_dir=$1
contrib_dir=`dirname $0`
examples_output_dir=$build_dir/examples_output

rm -rf $examples_output_dir
mkdir -p $examples_output_dir

for f in $build_dir/examples/*; do
    if [ -x "$f" -a ! -d "$f" ]; then
        echo $f
        ./$f -x | $contrib_dir/visualize.py -o $examples_output_dir
    fi
done
