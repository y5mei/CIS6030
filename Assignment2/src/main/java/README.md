# CIS6030 F22 A2 (Hadoop MapReduce)
Yaowen Mei (1177855) F22 

## How to compile and run these programs.
I basically changed only a few lines from the origional `WordCount.Java` program, so to compile and run will be pretty much the same as the [MapReduceTutorial from the hadoop official website](https://hadoop.apache.org/docs/stable/hadoop-mapreduce-client/hadoop-mapreduce-client-core/MapReduceTutorial.html#Purpose).

### 1. To compile
Assuming environment variables are set of the follows:
```
export JAVA_HOME=/usr/java/default
export PATH=${JAVA_HOME}/bin:${PATH}
export HADOOP_CLASSPATH=${JAVA_HOME}/lib/tools.jar
```
Compile `WordCountQ1.java` and `WordCountQ2.java` and create two jars:
```
// Create jar for question 1 as wcq1.
$ bin/hadoop com.sun.tools.javac.Main WordCountQ1.java
$ jar cf wcq1.jar WordCountQ1*.class

// Create jar for question 2 as wcq2
$ bin/hadoop com.sun.tools.javac.Main WordCountQ2.java
$ jar cf wcq2.jar WordCountQ2*.class
```

Assuming that you already have the input and out directories:
* `/user/joe/wordcount/input - input directory in HDFS`
* `/user/joe/wordcount/output - output directory in HDFS`

A1_data.txt as input:
```
$ bin/hadoop fs -ls /user/joe/wordcount/input/
/user/joe/wordcount/input/A1_data.txt

$ bin/hadoop fs -cat /user/joe/wordcount/input/A1_data.txt | head -n 3
nwlrbbmqb the information published in this undergraduate calendar outlines the 
hcdarzowk rules the undergraduate calendar is available in pdf format 
kyhiddqsc if you wish to link to undergraduate calendar please 
```

### To run the jars
Run question-1 applications:
```
$ bin/hadoop jar wcq1.jar WordCountQ1 /user/joe/wordcount/input/A1_data.txt /user/joe/wordcount/output/q1result
```

Check the output for question 1:
```
$ bin/hadoop fs -cat /user/joe/wordcount/output/q1result/part-r-00000

agriculturalcommunication	1
agriculturalcommunications	1
compassionateconsideration	1
contemporaryenvironmental	1
describingextracurricular	3
environmentalperspectives	10
extracurricularactivities	24
extracurricularexperience	2
extracurricularleadership	1
internationalbaccalaureate	1
interpersonalcommunication	2
outstandingextracurricular	1
participatedsignificantly	5
psychologicalcompassionate	2
universityextracurricular	6
```

Run question-2 applications:
```
$ bin/hadoop jar wcq2.jar WordCountQ2 /user/joe/wordcount/input/A1_data.txt /user/joe/wordcount/output/q2result
```

Check the output for question 2:
```
$ bin/hadoop fs -cat /user/joe/wordcount/output/q2result/part-r-00000

Total number: 	468820
```