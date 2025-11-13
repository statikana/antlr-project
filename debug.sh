JAVA_GEN_DIR="./gen_java"

# clear old stuff
rm -rf $JAVA_GEN_DIR/*

# generate files to target lang java
java -jar /usr/local/lib/antlr-4.13.2-complete.jar -Dlanguage=Java -o $JAVA_GEN_DIR/ -visitor ./main.g4

# compile java to .class files to be used by testrig
javac -cp ".:$JAVA_GEN_DIR:/usr/local/lib/antlr-4.13.2-complete.jar" -d $JAVA_GEN_DIR $JAVA_GEN_DIR/*.java

# can now call testrig
echo "Ready to debug [grun ...]"