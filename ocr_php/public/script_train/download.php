<?php
if(!empty($_FILES['file']))
{
    $file_path = sprintf("./img_train/%s",$_FILES['file']['name']);
    if(!move_uploaded_file($_FILES["file"]["tmp_name"],
        $file_path))
        echo $_FILES["file"]["error"];
    $cmd = "convert " . $file_path . " " . "./img_train/xi.ha.exp0.tif";
     shell_exec($cmd);
    $cmd = "sh ./img_train/makebox.sh chi_sim";
     shell_exec($cmd);
    $zipname  = time();
    $cmd = "zip ./img_train/" . $zipname . ".zip ./img_train/xi.ha.exp0.tif ./img_train/xi.ha.exp0.box";  
    shell_exec($cmd);
    ob_end_clean();
    $filename = "./img_train/" . $zipname . ".zip";
    header("Cache-Control: public"); 
    header("Content-Description: File Transfer"); 
    header('Content-disposition: attachment; filename='.basename($filename)); //文件名   
    header("Content-Type: application/zip"); //zip格式的   
    header("Content-Transfer-Encoding: binary"); //告诉浏览器，这是二进制文件    
    @readfile($filename);
}

