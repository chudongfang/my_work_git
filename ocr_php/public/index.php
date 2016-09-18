<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
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
    position:absolute;
    left:600px;
    top:100px 

  }
  .image{
    position:absolute;
    left:10px;
    top:60px 
  }
  .retext{
    position:absolute;
    top:370px 
  }

  .body{
    background-image: url("./img/background.jpg");
  }
  .text{
    text-align: center;       
  }
</style>
</head>

<body class = "body">
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
      <!--<ul class="nav navbar-nav">
        <li class="active"><a href="#">系统介绍</a></li>
        <li class="dropdown">
          <a href="#" class="dropdown-toggle" data-toggle="dropdown">Dropdown <span class="caret"></span></a>
          <ul class="dropdown-menu" role="menu">
            <li><a href="#">Action</a></li>
            <li><a href="#">Another action</a></li>
            <li><a href="#">Something else here</a></li>
            <li class="divider"></li>
            <li><a href="#">Separated link</a></li>
            <li class="divider"></li>
            <li><a href="#">One more separated link</a></li>
          </ul>
        </li>
      </ul>
-->
      <!--<form class="navbar-form navbar-left" role="search">
        <div class="form-group">
          <input type="text" class="form-control" placeholder="Search">
        </div>
        <button type="submit" class="btn btn-default">Submit</button>
      </form>
      -->
      <ul class="nav navbar-nav navbar-right">
        <li><a href="./introduce1.php">系统介绍</a></li>
        <li><a href="#">实验室介绍</a></li>
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





<div class = "form">
  <form  action="" method="post" enctype="multipart/form-data">
    <label for="file">上传图片:</label>
      <input type="file" name="file" id="file" /> 
      <br />
      <p>请选择语言类型(可多选)：</p> 
      <div class="checkbox">
        <label>
         <input type="checkbox" value="English" name ="mrbook[]" >
         English
        </label>
      </div>
      <div class="checkbox">
       <label>
        <input type="checkbox" value="中文" name ="mrbook[]">
        中文
       </label>
     </div>

     <div class="checkbox">
     <label>
       <input type="checkbox" value="Deutsch" name ="mrbook[]">
       Deutsch
     </label>
     </div>
     <div class="checkbox">
       <label>
       <input type="checkbox" value="한국의" name ="mrbook[]">
         한국의
       </label>
     </div>
      <input type="submit" name="submit" class="btn btn-success" value="Submit" />
</form>
</div>


<?php

    if(!empty($_FILES['file']))
    {
        $file_path = sprintf("./upload/%s",$_FILES['file']['name']);
        if(!move_uploaded_file($_FILES["file"]["tmp_name"],
            $file_path))
            echo $_FILES["file"]["error"];
        for($i=0 ;$i<count($_POST[mrbook]);$i++)
            if(strcmp($_POST[mrbook][$i],"English") == 0)
                $lan_type = sprintf("%s %s",$lan_type,"eng");
            else if(strcmp($_POST[mrbook][$i],"中文") == 0)
                $lan_type = sprintf("%s %s",$lan_type,"chi_sim");

    }


    require '../vendor/autoload.php';
    $ocr = new \TesseractOCR($file_path);
    $string = $ocr ->lang($lan_type) ->run();

?>
<div class = "image">
   <p>
    <img src=<?php
             if(!empty($file_path)) echo $file_path;
             else  echo './img/180.jpg'; ?>
         width="440" height="300" />
   </p>
</div>

<div class = "retext" style ="width :100%">
<p>识别结果：</p>
<textarea class="form-control" rows="10"><?php echo $string; ?></textarea>
</div>





</body>
</html>
