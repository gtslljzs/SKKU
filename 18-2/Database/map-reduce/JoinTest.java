import java.io.IOException;
import java.util.ArrayList;
import java.nio.file.FileSystem;
// import MultipleInputs.Join;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Mapper.Context;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.input.MultipleInputs;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class JoinTest {

    public static class JoinMapper1 extends Mapper<Object, Text, Text, Text> {

        public void map(Object key, Text value, Context context)
                throws IOException, InterruptedException {

            String record = value.toString(); // Read the input text value
            String[] parts = record.split(","); // Split the input

            // key: Select the B attribute that we want to join
            // values: Add "R" string as a sign that where the mapped lists are came from
            // values: Select the 'A' attribute that we want to print
            context.write(new Text(parts[1]), new Text("R" + parts[0]));
        }
    }

    public static class JoinMapper2 extends Mapper<Object, Text, Text, Text> {
        
        public void map(Object key, Text value, Context context)
                throws IOException, InterruptedException {

            String record = value.toString(); // Read the input text value
            String[] parts = record.split(","); // Split the input

            // key: Select the B attribute that we want to join
            // values: Add "S" string as a sign that where the mapped lists are came from
            // values: Select the 'C' attribute that we want to print
            context.write(new Text(parts[0]), new Text("S," + parts[1]));
        }
    }

    public static class ReduceJoinReducer extends Reducer<Text, Text, Text, Text> {

        public void reduce(Text key, Iterable<Text> values, Context context)
                throws IOException, InterruptedException {
            
            ArrayList<String> name = new ArrayList<String>();
            String dept = "";

            // Each element is like ("R", name) from R, or ("S", dept) from S
            for(Text t: values) {

                // By splitting, we will obtain a sign string (parts[0]) and a string to print parts[1])
                String parts[] = t.toString().split(",");

                // Find the list of name strings and store them into the name arraylist
                if(parts[0].equals("R"))
                    name.add(parts[1]);
                
                // Find the dept for a given key
                else if(parts[0].equals("S"))
                    dept = parts[1];
            }

            // Print each (name, dept) pair
            for(String s: name)
                context.write(new Text(s), new Text(dept));

        }
    }

    public static void main(String[] args) throws Exception {
    
        Configuration conf = new Configuration();

        // String[] files = new GenericOptionsParser(conf, args).getRemainingArgs();
        // Path p1=new Path(files[0]);
        // Path p2=new Path(files[1]);
        // Path p3=new Path(files[2]);
        // FileSystem fs = FileSystem.get(conf);
        // if(fs.exists(p3)){
        //     fs.delete(p3, true);
        // }

        Job job = Job.getInstance(conf, "JoinTest");
        job.setJarByClass(JoinTest.class);
        MultipleInputs.addInputPath(job, new Path(args[0]), TextInputFormat.class, JoinMapper1.class);
        MultipleInputs.addInputPath(job, new Path(args[1]), TextInputFormat.class, JoinMapper2.class);
        FileOutputFormat.setOutputPath(job, new Path(args[2]));
        job.setReducerClass(ReduceJoinReducer.class);
        // job.setMapOutputKeyClass(Text.class);
        // job.setMapOutputValueClass(Text.class);
        job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
        System.exit(job.waitForCompletion(true) ? 0 : 1);

    
    }

}