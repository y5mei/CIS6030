fun main(args: Array<String>) {
//    val customer = Customer("Phase")
//    println(customer.name)
//    println(customer.placeOrder())
//    println("Hello World!")
//    var args = arrayOf("./core-site.xml","output")

    println("This is inside of a Ktx program")
    println("Program arguments are: ${args.joinToString()}")
    WordCount.main(args)
    println("Program has been finished!")
}

// Steps to run sample program
// 1. CD to hadoop root directory (所有的 hadoop 命令都在这个文件夹里):
// cd /usr/local/cellar/hadoop/3.3.4/libexec
// list all the curr files/dirs, make sure there is an input direc, and there is an output dir
// hdfs dfs -ls
