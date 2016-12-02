<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,height=device-height,inital-scale=1.0,maximum-scale=1.0,user-scalable=no;">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-status-bar-style" content="black">
<meta name="format-detection" content="telephone=no">
<title>OCR文本识别系统</title>
<!-- 新 Bootstrap 核心 CSS 文件 -->
<link rel="stylesheet" href="http://cdn.bootcss.com/bootstrap/3.3.0/css/bootstrap.min.css">
<!-- 可选的Bootstrap主题文件（一般不用引入） -->
<link rel="stylesheet" href="http://cdn.bootcss.com/bootstrap/3.3.0/css/bootstrap-theme.min.css">
<!-- jQuery文件。务必在bootstrap.min.js 之前引入 -->
<script src="http://cdn.bootcss.com/jquery/1.11.1/jquery.min.js"></script>

<!-- 最新的 Bootstrap 核心 JavaScript 文件 -->
<script src="http://cdn.bootcss.com/bootstrap/3.3.0/js/bootstrap.min.js"></script>


<style type="text/css">
  .form{
    float: right;
    left:600px;
    top:100px 

  }
  .image{
    float: left;
    left:10px;
    top:60px 
  }
  .retext{
    position:absolute;
    top :70px;
  }
  .download{
    position:absolute;
    left:450px;
    top :330px;
  }

  .body{
    background-image: url("./img/background.jpg");
  }
  .text{
    text-align: center;       
  }
</style>
</head>

<body>
<?php //$file_path = './img/180.jpg'?>
<nav class="navbar navbar-inverse" role="navigation">
  <div class="container-fluid">
    <!-- Brand and toggle get grouped for better mobile display -->
    <div class="navbar-header">
      <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
        <span class="sr-only">Toggle navigation</span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>
      <a class="navbar-brand" href="index.php">OCR文本识别系统</a>
    </div>

    <!-- Collect the nav links, forms, and other content for toggling -->
    <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
      <ul class="nav navbar-nav navbar-right">
        <li><a href="./introduce1.php">系统介绍</a></li>
        <li><a href="./introduce1.php">识别样例</a></li>
        <li><a href="./train.php">训练</a></li>
        <li class="dropdown">
          <a href="#" class="dropdown-toggle" data-toggle="dropdown">参考资源 <span class="caret"></span></a>
          <ul class="dropdown-menu" role="menu">
            <li><a href="https://github.com/thiagoalessio/tesseract-ocr-for-php">tesseract-ocr-for-php</a></li>
            <li><a href="http://getbootstrap.com/">Bootstrap</a></li>
            <li><a href="http://www.w3school.com.cn/">W3School</a></li>
            <li class="divider"></li>
            <li><a href="https://github.com/tesseract-ocr/tesseract">tesseract-ocr</a></li>
          </ul>
        </li>
      </ul>
    </div><!-- /.navbar-collapse -->
  </div><!-- /.container-fluid -->
</nav>

<div class="container">
  <div class="row">
    <div class="col-md-4">
     <div>
      <form  action="download.php" method="post" enctype="multipart/form-data">
       <!-- <input type="file" name="file" id="file-1" class="inputfile inputfile-1"/>
    <label for="file-1"><svg xmlns="http://www.w3.org/2000/svg" width="150" height="100" viewBox="0 0 20 17"><path d="M10 0l-5.2 4.9h3.3v5.1h3.8v-5.1h3.3l-5.2-4.9zm9.3 11.5l-3.2-2.1h-2l3.4 2.6h-3.5c-.1 0-.2.1-.2.1l-.8 2.3h-6l-.8-2.2c-.1-.1-.1-.2-.2-.2h-3.6l3.4-2.6h-2l-3.2 2.1c-.4.3-.7 1-.6 1.5l.6 3.1c.1.5.7.9 1.2.9h16.3c.6 0 1.1-.4 1.3-.9l.6-3.1c.1-.5-.2-1.2-.7-1.5z"/></svg> <span>选择文件&hellip;</span></label>-->
        <label for="file">上传训练图片:</label>
        <input type="file" name="file" id="file" /> 
        <input type="submit" name="submit" class="btn btn-success" value="上传" />
      </form>
     </div>
    </div>
    <div class="col-md-4">
     <div>
     <form  action="" method="post" enctype="multipart/form-data">
          <!-- <input type="file" name="sucai" id="file-1" class="inputfile inputfile-1"/>
    <label for="file-1"><svg xmlns="http://www.w3.org/2000/svg" width="150" height="100" viewBox="0 0 20 17"><path d="M10 0l-5.2 4.9h3.3v5.1h3.8v-5.1h3.3l-5.2-4.9zm9.3 11.5l-3.2-2.1h-2l3.4 2.6h-3.5c-.1 0-.2.1-.2.1l-.8 2.3h-6l-.8-2.2c-.1-.1-.1-.2-.2-.2h-3.6l3.4-2.6h-2l-3.2 2.1c-.4.3-.7 1-.6 1.5l.6 3.1c.1.5.7.9 1.2.9h16.3c.6 0 1.1-.4 1.3-.9l.6-3.1c.1-.5-.2-1.2-.7-1.5z"/></svg> <span>选择文件&hellip;</span></label>-->
      <label for="file">上传训练素材:</label>
      <input type="file" name="sucai" id="file" /> 
      <input type="submit" name="sucai" class="btn btn-success" value="上传" />
     </form>
     </div>
    </div>
    <div class="col-md-4">
     <div>
       <p>
         <a href="./source_tools/jTessBoxEditor.zip">训练工具下载for(linux)</a>
       </p>
       <p>
         <a href="./source_tools/test.tif">测试图片</a>
       </p>
       <p>
         <a href="./source_tools/jTessBoxEditor_win.rar">训练工具下载for(windows)</a>
       </p>
     </div>
    </div>
  </div>
</div>
<?php

if(!empty($_FILES['sucai']))
{
    $file_path = sprintf("/home/wwwroot/default/ocr_php/public/data_train/%s",$_FILES['sucai']['name']);
    if(!move_uploaded_file($_FILES["sucai"]["tmp_name"],
        $file_path))
        echo $_FILES["sucai"]["error"];
    $path = "/home/wwwroot/default/ocr_php/public/data_train/";
    $sucaizipname = time(); 
    $cmd = "sh ./data_train/unzip.sh " . strtok($_FILES['sucai']['name'],".") . " " . $sucaizipname;
    echo  $sucaizipname;
    echo shell_exec($cmd);
    $cmd = "sh ./data_train/" . $sucaizipname . "/" . "creat.sh chi_sim "  .  "chi_" . $sucaizipname. " " . $sucaizipname . "/";
    echo shell_exec($cmd);
   // $cmd = "sh ./data_train/move.sh /home/wwwroot/default/ocr_php/public/data_train/" .  $sucaizipname   . "/" ."chi_haha.traineddata /usr/local/share/tessdata/";
   // echo $cmd;
   // echo shell_exec($cmd);
}


?>




<body>
</html>
