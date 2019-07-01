import java.io.IOException;
import java.util.ArrayList;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.MultipleInputs;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
 
public class ReduceJoin {

	public static class EmpMapper extends Mapper <Object, Text, Text, Text> {
		public void map(Object key, Text value, Context context)
				throws IOException, InterruptedException {
 			String record = value.toString();
			String[] parts = record.split(",");
			context.write(new Text(parts[7]), new Text("emp," + parts[1]));
		}
 	}
 
	public static class DeptMapper extends Mapper <Object, Text, Text, Text> {
		public void map(Object key, Text value, Context context) 
				throws IOException, InterruptedException {
			String record = value.toString();
			String[] parts = record.split(",");
			context.write(new Text(parts[0]), new Text("dept," + parts[2]));
		}
	}
 
 	public static class ReduceJoinReducer extends Reducer <Text, Text, Text, Text> {
		public void reduce(Text key, Iterable<Text> values, Context context)
				throws IOException, InterruptedException {
			
			ArrayList<String> ename = new ArrayList<String>();
			String loc = "";

			for (Text t : values) { 
				String parts[] = t.toString().split(",");
				if (parts[0].equals("emp")) ename.add(parts[1]);
				else if (parts[0].equals("dept")) loc = parts[1];
			}
			for (String s : ename) context.write(new Text(s), new Text(loc));
 		}
 	}
 
	public static void main(String[] args) throws Exception {
		Configuration conf = new Configuration();
		Job job = new Job(conf, "Reduce-side join");
		job.setJarByClass(ReduceJoin.class);
		job.setReducerClass(ReduceJoinReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
  
		MultipleInputs.addInputPath(job, new Path(args[0]),TextInputFormat.class, EmpMapper.class);
		MultipleInputs.addInputPath(job, new Path(args[1]),TextInputFormat.class, DeptMapper.class);
		Path outputPath = new Path(args[2]);
  
		FileOutputFormat.setOutputPath(job, outputPath);
		outputPath.getFileSystem(conf).delete(outputPath);
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
