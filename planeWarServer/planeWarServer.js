var mysql  = require('mysql');  
var http = require('http');
var url = require('url');
var util = require('util');
 
var connection = mysql.createConnection({     
  host     : 'localhost',       
  user     : 'root',              
  password : 'mysqlxjl123',       
  port: '3306',                   
  database: 'gamedata', 
}); 
 
connection.connect();
 
http.createServer(function(req, res){
 
    // 解析 url 参数
	console.log('linked');
    var params = url.parse(req.url, true).query;
	var type=params.type; 
	var name=params.user; 
	console.log(name);
	
	switch (type) 
	{ 
	//处理注册请求
	  case 'register':
	  console.log('register request');
	  var findUser1='select count(*) as num from data where user =?';
	  var findUserParams1=[name];
	  
	  connection.query(findUser1,findUserParams1,function (err, result) {
		res.writeHead(200, {'Content-Type': 'text/plain'});
        if(err){
          console.log('[SELECT ERROR] - ',err.message);
          return;
        }
		if (result[0]["num"]>0)
		{
		  res.write("userExist");
		  res.end();
		}
		else
		{
		  var  password=params.password;
		  var  addUser = 'INSERT INTO data(id,user,password,money,scole,toolNumber) VALUES(0,?,?,?,?,?)';
		  var  addUserParams = [name, password,1000, 0,10];
		  
		  connection.query(addUser,addUserParams,function (err, result) {
			  if(err)
			  {
				 console.log('[INSERT ERROR] - ',err.message);
				 return;
			  }
			  else
			  {
				  console.log('register succeed');
				  res.write("succeed");
				  res.end();
			  }
				  });
		}

		});
	  break; 
	  
	  //处理登录请求
	  case 'enter':
	  console.log('enter request');
	  var findUser2='select count(*) as num from data where user =?';
	  var findUserParams2=[name];
	  console.log(name);
	  
	  connection.query(findUser2,findUserParams2,function (err, result) {
		res.writeHead(200, {'Content-Type': 'text/plain'});
        if(err){
          console.log('[SELECT ERROR] - ',err.message);
          return;
        }
		if (result[0]["num"]==0)
		{
		  res.write("noUser");
		  console.log("noUser");
		  res.end();
		}
		else
		{
		  var  password=params.password;
		  console.log(password);
		  var  findPassword = 'select password from data where user =?';
		  var  findPasswordParams = [name];
		  
		  connection.query(findPassword,findPasswordParams,function (err, result) {
			  if(err)
			  {
				 console.log('[INSERT ERROR] - ',err.message);
				 return; 
			  }
			  else
			  {
				  if(result[0]["password"]==password)
				  {
					  res.write("succeed");
					  console.log("enter succeed");
					  res.end();
				  }
				  else
				  {
					  console.log('passWordWrong');
					  res.write("passWordWrong");
					  res.end();
				  }
			  }
				  });
		}

		});
	  break; 
	  
	  //处理商店信息同步请求
	  case 'store':
	  console.log('store request');
	  var storeFind='select money,toolNumber from data where user =?';
	  var storeFindParams=[name];
	  
	  connection.query(storeFind,storeFindParams,function (err, result) {
			  if(err)
			  {
				 console.log('[INSERT ERROR] - ',err.message);
				 return;
			  }
			  else
			  {
				  console.log('store get the data');
				  res.writeHead(200, {"Content-Type": "application/json"});
				  res.write(JSON.stringify(result[0]));//这是一个json格式行，包括money和toolNumber
				  res.end();
			  }
				  });
	  
	  break; 
	  
	  //处理购买请求
	  case 'buy':
	  console.log('buy request');
	  var money=params.money; 
	  var toolNumber=params.toolNumber;
	  var buyUpdate='UPDATE data SET money = ?,toolNumber = ? WHERE user = ?';
	  var buyUpdateParams=[parseInt(money),parseInt(toolNumber),name];
	  
	  connection.query(buyUpdate,buyUpdateParams,function (err, result) {
			  if(err)
			  {
				 console.log('[INSERT ERROR] - ',err.message);
				 return;
			  }
			  else
			  {
				  console.log('buy succeed');
				 res.writeHead(200, {'Content-Type': 'text/plain'});
				 res.write("succeed");
				 res.end();
			  }
				  });

	  break; 
	  
	  //处理得分纪录请求
	  case 'record':
	  console.log('record request');
	  console.log(params.score);
	  var  score=parseInt(params.score);
	  console.log(params.toolNumber);
	  var  toolNumber=parseInt(params.toolNumber);
	  var  findScore = 'select scole from data where user =?';
	  var  findScoreParams = [name];
		 
	  connection.query(findScore,findScoreParams,function (err, result) {
		  res.writeHead(200, {'Content-Type': 'text/plain'});
		  if(err)
		  {
			 console.log('[INSERT ERROR] - ',err.message);
			 return; 
		  }
		  else
		  {
			  if(result[0]["scole"]>score)
			  {
				  score=result[0]["scole"];
			  }
		  }
		  
		  var dataUpdate='UPDATE data SET scole =?,toolNumber=? WHERE user = ?';
		  var dataUpdateParams=[score,toolNumber,name];
		  connection.query(dataUpdate,dataUpdateParams,function (err, result) {
				  if(err)
				  {
					 console.log('[INSERT ERROR] - ',err.message);
					 return;
				  }
				  else
				  {
					 console.log('gamedata recorded');
					 res.write("succeed");
					 res.end();
				  }
					  });
			  
				  });   
	  break; 
	  
	  //处理排行榜请求
	  case 'rank':
	  console.log('rank request');
	  var rankFind='select user,scole,rank from \
	  (select user,scole,@crank:=if(@prescole=scole,@crank,@incrank) as rank,@prescole:=scole,@incrank:=@incrank+1 from\
	  data,(select @crank:=0,@prescole:=null,@incrank:=1)r order by scole desc)s limit 10';
	  var myRankFind='select user,scole,rank from\
	  (select user,scole,@crank:=if(@prescole=scole,@crank,@incrank) as rank,@prescole:=scole,@incrank:=@incrank+1 from\
	  data,(select @crank:=0,@prescole:=null,@incrank:=1)q order by scole desc)s where user=?';
	  var myRankFindParams=[name];
	  var myRank;
	  
	  connection.query(myRankFind,myRankFindParams,function (err, myResult) {
			  if(err)
			  {
				 console.log('[INSERT ERROR] - ',err.message);
				 return;
			  }
			  else
			  {
				  myRank=myResult[0];
			  }
			  
			  connection.query(rankFind,function (err, result) {
				  if(err)
				  {
					 console.log('[INSERT ERROR] - ',err.message);
					 return;
				  }
				  else
				  {
					  console.log('get rank succeed');
					  res.writeHead(200, {"Content-Type": "application/json"});
					  var jrank={"myRank":myRank,"rankFirst":result};
					  console.log(jrank);
					  res.write(JSON.stringify(jrank));
					  res.end();
				  }
					  });
				  
				  });
	 		  
	  break; 
	  //获取游戏数据
	  case 'getdata':
	  console.log('getdata request');
	  var getdataFind='select toolNumber from data where user =?';
	  var getdataFindParams=[name];
	  
	  connection.query(getdataFind,getdataFindParams,function (err, result) {
			  if(err)
			  {
				 console.log('[INSERT ERROR] - ',err.message);
				 return;
			  }
			  else
			  {
				  console.log('game get the data');
				  res.writeHead(200, {'Content-Type': 'text/plain'});
				  res.write(result[0]["toolNumber"].toString());
				  res.end();
			  }
				  });
	  
	  break; 
	}
 
}).listen(3000);

console.log('GameServer running');

//connection.end();




