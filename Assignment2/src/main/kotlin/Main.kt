fun main(args: Array<String>) {

    println("This is inside of a Ktx program")
    println("Program arguments are: ${args.joinToString()}")
    WordCount.main(args)
    println("Program has been finished!")
}
